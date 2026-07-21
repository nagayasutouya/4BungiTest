#include "Skeleton.h"
#include "Animation.h"
#include <fstream>

// ファイルの読み込み
void Skeleton::load(const std::string& file_name) {
    // ファイルオープン
    std::ifstream file{ file_name };
    if (!file) {
        throw std::runtime_error("can not open " + file_name);
    }
    // 消去
    clear();
    // ボーン数の読み込み
    int num_bone = 0;
    file >> num_bone;
    for (int i = 0; i < num_bone; ++i) {
        // ボーンの読み込み
        Bone bone;
        file >> bone.name;
        file >> bone.id;
        file >> bone.parent_id;
        file >> bone.parent_offset.x >> bone.parent_offset.y >> bone.parent_offset.z;
        file >> bone.position.x >> bone.position.y >> bone.position.z;
        bones_.push_back(bone);
    }
}

// ボーン数を返す
std::size_t Skeleton::bone_count() const {
    return bones_.size();
}

// ボーン名を取得
const std::string& Skeleton::bone_name(int index) const {
    return bones_[index].name;
}

// 指定アニメーション・フレームのローカル行列を計算する
void Skeleton::compute_local_matrices(const Animation& animation, float frame_no, Matrix4x4 local[]) const {
    for (const auto& bone : bones_) {
        local[bone.id] = animation.get_key_frame(bone.name, frame_no).matrix();
    }
}

// ワールド行列を計算する
void Skeleton::compute_world_matrices(const Matrix4x4& parent,
    const Matrix4x4 local[], Matrix4x4 world[]) const {
    // ルートボーンのワールド変換行列計算
    world[0] = local[0] * Matrix4x4::translate(bones_[0].parent_offset) * parent;
    // 子ボーンのワールド変換行列計算
    for (std::size_t i = 1; i < bones_.size(); ++i) {
        world[i] = local[i]
            * Matrix4x4::translate(bones_[i].parent_offset)
            * world[bones_[i].parent_id];
    }
}

// スキニング用の変換行列を計算する
void Skeleton::compute_skinned_matrices(const Matrix4x4 world[], Matrix4x4 skinned[]) const {
    for (const auto& bone : bones_) {
        skinned[bone.id] = Matrix4x4::translate(-bone.position) * world[bone.id];
    }
}

// 2つのアニメーションをブレンドした行列を計算する
void Skeleton::compute_blended_matrices(
    const Animation& anim1, float frame1,
    const Animation& anim2, float frame2,
    float weight,
    Matrix4x4 local[]) const {
    for (const auto& bone : bones_) {
        Animation::KeyFrame k1 = anim1.get_key_frame(bone.name, frame1);
        Animation::KeyFrame k2 = anim2.get_key_frame(bone.name, frame2);
        Animation::KeyFrame blended = Animation::KeyFrame::lerp(k1, k2, weight);
        local[bone.id] = blended.matrix();
    }
}

// 消去
void Skeleton::clear() {
    bones_.clear();
}