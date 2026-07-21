#include "AnimatedMesh.h"
#include <cmath>
#include <algorithm>

// 補間フレーム数
const float LerpTime{ 10.0f };

// コンストラクタ
AnimatedMesh::AnimatedMesh(unsigned int id, unsigned int motion, bool loop, unsigned int num_bones) :
    id_{ id },
    motion_{ motion },
    motion_timer_{ 0.0f },
    motion_loop_{ loop },
    transform_{ Matrix4x4::identity()},
    prev_motion_{ motion },
    prev_motion_timer_{ 0.0f },
    lerp_timer_{ 0.0f },
    local_bone_matrices_{ num_bones },
    bone_matrices_{ num_bones },
    skinned_matrices_{num_bones} {
}

// 更新
void AnimatedMesh::update(float delta_time) {
    // モーションタイマの更新
    motion_timer_ += delta_time;
    // ループアニメーションか？
    if (motion_loop_) {
        // モーションタイマをループさせる
        motion_timer_ = std::fmod(motion_timer_, motion_end_time());
    } else {
        // モーションタイマをクランプする
        motion_timer_ = Mathf::Min(motion_timer_, motion_end_time() - 1.0f);
    }
    // 補間タイマの更新（LerpTime以上にならないようにクランプする）
    lerp_timer_ = Mathf::Min(lerp_timer_ + delta_time, LerpTime);
}

// 描画
void AnimatedMesh::draw() const {
    Graphics::set_world_matrix(transform_);
    Graphics::set_skinned_matrices(id_,skinned_matrices_.data());
    Graphics::draw_skinned_mesh(id_);
}

// モーションの変更
void AnimatedMesh::change_motion(unsigned int motion, bool loop) {
    // 現在と同じモーションの場合は何もしない
    if (motion_ == motion) return;

    // 補間中（前半）は、前回のモーションを更新しないようにする
    if (lerp_timer_ > (LerpTime * 0.5f)) {
        // 前回のモーション番号とモーション時間を保存
        prev_motion_ = motion_;
        prev_motion_timer_ = motion_timer_;
        // 補間中のタイマのリセット
        lerp_timer_ = 0.0f;
    }

    // モーションの変更
    motion_ = motion;
    // モーションタイマの初期化
    motion_timer_ = 0.0f;
    // モーションループフラグの設定
    motion_loop_ = loop;
}

// 変換行列を設定する
void AnimatedMesh::transform(const Matrix4x4& matrix) {
    transform_ = matrix;
    // ボーンごとのローカル変換行列を計算
    Graphics::compute_blended_matrices( id_, prev_motion_, prev_motion_timer_,motion_, motion_timer_,lerp_timer_ / LerpTime,local_bone_matrices_.data());
    // ボーンの変換行列を計算
    Graphics::compute_world_matrices(id_, local_bone_matrices_.data(), bone_matrices_.data());
    Graphics::compute_skinned_matrices(id_, bone_matrices_.data(), skinned_matrices_ .data());
}

// モーションが終了しているか？
bool AnimatedMesh::is_end_motion() const {
    // ループモーションは終了しない
    if (motion_loop_) return false;
    // 終了しているか？
    return motion_timer_ >= (motion_end_time() - 1.0f);
}

// 現在のモーションの再生時間を取得
float AnimatedMesh::motion_time() const {
    return motion_timer_;
}

// モーションの再生時間を設定
void AnimatedMesh::motion_time(float time) {
    motion_timer_ = time;
}

// モーションの終了時間を取得
float AnimatedMesh::motion_end_time() const {
    return Graphics::get_skinned_mesh_last_frame(id_, motion_);
}

// ボーンのワールド変換行列を取得　
Matrix4x4 AnimatedMesh::bone_matrices(int bone_no) const {
    return bone_matrices_[bone_no] * transform_;
}