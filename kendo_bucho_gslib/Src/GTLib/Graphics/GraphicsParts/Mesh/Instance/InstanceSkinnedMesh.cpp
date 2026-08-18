#include "InstanceSkinnedMesh.h"
#include "GTLib/Graphics/GraphicsParts/GraphicsDevice.h"

// コンストラクタ
InstanceSkinnedMesh::InstanceSkinnedMesh(GraphicsDevice* device) :
    device_{ device }, mesh_{ device }{
}
// デストラクタ
InstanceSkinnedMesh::~InstanceSkinnedMesh() {
    clear();
}
// ファイル読み込み
void InstanceSkinnedMesh::load(const std::string& mesh_path, const std::string& skeleton_path, const std::string& animation_path, int instance_count) {
    mesh_.load(mesh_path);
    Skeleton skeleton;
    skeleton.load(skeleton_path);
    for (int i = 0; i <instance_count ; i++) {
        skeletons_.push_back(skeleton);
    }
    animation_set_.load(animation_path);
    instanse_count_ = instance_count;
}
// 指定アニメーション・フレームのローカル行列を計算する
void InstanceSkinnedMesh::compute_local_matrices(int data_count,LocalMat local[]) const {
    for (int i = 0; i < data_count; i++) {
        skeletons_[i].compute_local_matrices(animation_set_.get(local[i].anim_id), local[i].frame_no, local[i].local);
    }
}
// ワールド行列を計算する
void InstanceSkinnedMesh::compute_world_matrices(int data_count, WorldMat world[]) const {
    for (int i = 0; i < data_count; i++) {
        skeletons_[i].compute_world_matrices(world[i].parent, world[i].local, world[i].world);
    }
}
// スキニング行列を計算する
void InstanceSkinnedMesh::compute_skinned_matrices(int data_count, SkinnedMat skinned[]) const {
    for (int i = 0; i < data_count; i++) {
        skeletons_[i].compute_skinned_matrices(skinned[i].world, skinned[i].skinned);
    }
}
// 2つのアニメーションをブレンドしたローカル行列を計算する
void InstanceSkinnedMesh::compute_blended_matrices(int data_count, BlendedMat blended[]) const {
    for (int i = 0; i < data_count; i++) {
        skeletons_[i].compute_blended_matrices(animation_set_.get(blended[i].anim_id1), blended[i].frame1, animation_set_.get(blended[i].anim_id2), blended[i].frame2, blended[i].weight, blended[i].local);
    }
}
// スキニング行列をセットする
void InstanceSkinnedMesh::set_skinned_matrices(const Matrix4x4 skinned[],int instanse_id) const {
    device_->set_constant_buffer(4, skinned, sizeof(Matrix4x4) * bone_count(instanse_id));
}
// 全ボーンデータをセットする
void InstanceSkinnedMesh::set_all_bone_data(const Matrix4x4 bone[],int bone_count) {
    bone_srv_.upload(bone ,bone_count ,current_instance_count_);
}
// 全オブジェクトデータをセットする
void InstanceSkinnedMesh::set_all_object_data(InstanceObject object[],int object_count) {
    objects_srv_.upload(object,object_count,current_instance_count_);
}
// 描画する
void InstanceSkinnedMesh::draw(unsigned int instance_count) const {
    // メッシュ描画
    mesh_.draw(instance_count);
}
// ボーン数を返す
std::size_t InstanceSkinnedMesh::bone_count(int instance_id) const {
    return skeletons_[instance_id].bone_count();
}
// 指定アニメーションの終了フレームを返す
float InstanceSkinnedMesh::last_frame(int anim_id) const {
    return animation_set_.get(anim_id).last_frame();
}
// 消去
void InstanceSkinnedMesh::clear() {
    mesh_.clear();
    skeletons_.clear();
    animation_set_.clear();
}