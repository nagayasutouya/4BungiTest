#include "InstanceAnimatedMesh.h"
#include <cmath>
#include <algorithm>

// 補間フレーム数
const float InstanceLerpTime{ 10.0f };

// コンストラクタ
InstanceAnimatedMesh::InstanceAnimatedMesh(unsigned int id, unsigned int instance_count_max, unsigned int motion, bool loop, unsigned int num_bones) :id_{ id },transform_{ instance_count_max},local_bone_data_{ instance_count_max },bone_matrices_{ instance_count_max },skinned_matrices_{ instance_count_max },instance_count_max_{instance_count_max} {
    int bone = (int)Graphics::get_instance_skinned_mesh_bone_count(id);
    //データコンテナの作成
    for (int i = 0; i < instance_count_max; i++) {
        skinned_matrices_[i].skinned = new Matrix4x4[num_bones];
        bone_matrices_[i].world = new Matrix4x4[num_bones];
        local_bone_data_[i].local = new Matrix4x4[num_bones];
        transform_[i].bone_offset_ = bone * i;
    }
    instance_count_= instance_count_max_;
}
// 更新
void InstanceAnimatedMesh::update(float delta_time) {
    for (int i = 0; i < instance_count_; i++) {
        //オブジェクトがないならスキップ
        if (local_bone_data_[i].is_empty)continue;
        // モーションタイマの更新
        local_bone_data_[i].current_frame += delta_time;
        // ループアニメーションか？
        if (local_bone_data_[i].loop) {
            // モーションタイマをループさせる
            local_bone_data_[i].current_frame = std::fmod(local_bone_data_[i].current_frame, motion_end_time(i));
        }
        else {
            // モーションタイマをクランプする
            local_bone_data_[i].current_frame = Mathf::Min(local_bone_data_[i].current_frame, motion_end_time(i) - 1.0f);
        }
        // 補間タイマの更新（LerpTime以上にならないようにクランプする）
        local_bone_data_[i].lerp = Mathf::Min(local_bone_data_[i].lerp + delta_time, InstanceLerpTime);
    }
}
// 描画
void InstanceAnimatedMesh::draw() const {
    Graphics::set_instance_skinned_matrices(id_, instance_count_, skinned_matrices_.data());
    Graphics::draw_instance_skinned_mesh(id_,instance_count_,transform_.data());
}
// モーションの変更
void InstanceAnimatedMesh::change_motion(int instance_id,unsigned int motion, bool loop) {
    // 現在と同じモーションの場合は何もしない
    if (local_bone_data_[instance_id].is_empty||local_bone_data_[instance_id].current_motion == motion) return;

    // 補間中（前半）は、前回のモーションを更新しないようにする
    if (local_bone_data_[instance_id].lerp > (InstanceLerpTime * 0.5f)) {
        // 前回のモーション番号とモーション時間を保存
        local_bone_data_[instance_id].prev_motion = local_bone_data_[instance_id].current_motion;
        local_bone_data_[instance_id].prev_frame = local_bone_data_[instance_id].current_frame;
        // 補間中のタイマのリセット
        local_bone_data_[instance_id].lerp = 0.0f;
    }

    // モーションの変更
    local_bone_data_[instance_id].current_motion = motion;
    // モーションタイマの初期化
    local_bone_data_[instance_id].current_frame = 0.0f;
    // モーションループフラグの設定
    local_bone_data_[instance_id].loop = loop;
}
// 変換行列を設定する
void InstanceAnimatedMesh::transform(int instance_id, const Matrix4x4& matrix) {
    transform_[instance_id].world_ = matrix;
    // ボーンごとのローカル変換行列を計算
    Graphics::compute_blended_matrices_instance(id_,instance_id ,local_bone_data_[instance_id],InstanceLerpTime);
    // ボーンの変換行列を計算
    Graphics::compute_world_matrices_instance(id_, instance_id,local_bone_data_[instance_id].local, bone_matrices_[instance_id].world);
    Graphics::compute_skinned_matrices_instance(id_, instance_id, bone_matrices_[instance_id].world, skinned_matrices_[instance_id].skinned);
}
// モーションが終了しているか？
bool InstanceAnimatedMesh::is_end_motion(int instance_id) const {
    // ループモーションは終了しない
    if (local_bone_data_[instance_id].is_empty||local_bone_data_[instance_id].current_motion) return false;
    // 終了しているか？
    return local_bone_data_[instance_id].current_frame >= (motion_end_time(instance_id) - 1.0f);
}
// 現在のモーションの再生時間を取得
float InstanceAnimatedMesh::motion_time(int instance_id) const {
    return (local_bone_data_[instance_id].is_empty)?0.0f:local_bone_data_[instance_id].current_frame;
}
// モーションの再生時間を設定
void InstanceAnimatedMesh::motion_time(int instance_id, float time) {
    if(!local_bone_data_[instance_id].is_empty)local_bone_data_[instance_id].current_frame = time;
}
// モーションの終了時間を取得
float InstanceAnimatedMesh::motion_end_time(int instance_id) const {
    return (local_bone_data_[instance_id].is_empty) ? 0.0f : Graphics::get_instance_skinned_mesh_last_frame(id_, local_bone_data_[instance_id].current_motion);
}
// ボーンのワールド変換行列を取得　
Matrix4x4 InstanceAnimatedMesh::bone_matrices(int instance_id,int bone_no) const {
    return (local_bone_data_[instance_id].is_empty) ? Matrix4x4::identity() : bone_matrices_[instance_id].world[bone_no] * transform_[instance_id].world_;
}
//現在の描画数を設定
void InstanceAnimatedMesh::set_instance_count(int count) {
    instance_count_ = count;
    instance_count_ = Mathf::CLAMP(instance_count_,0,instance_count_max_);
}
//オブジェクトを追加
int InstanceAnimatedMesh::add_instnace() {
    int result = -1;
    for (int i = 0; i < instance_count_max_;i++) {
        //枠が空いてたらそこに代入
        if (local_bone_data_[i].is_empty) {
            result = i;
            local_bone_data_[i].is_empty = false;
            break;
        }
    }
    return result;
}
//オブジェクトを除外
void InstanceAnimatedMesh::remove_instance(int id) {
    //範囲外は無視
    if (id >= instance_count_max_ || id < 0)return;
    BlendedMat mat;
    local_bone_data_[id] = mat;
}