#include "InstanceSkinnedMesh.h"
#include "GTLib/Graphics/GraphicsParts/GraphicsDevice.h"

// コンストラクタ
InstanceSkinnedMesh::InstanceSkinnedMesh(GraphicsDevice* device) :device_{ device }, mesh_{ device }{}
// デストラクタ
InstanceSkinnedMesh::~InstanceSkinnedMesh() {
    clear();
}
// ファイル読み込み
void InstanceSkinnedMesh::load(const std::string& mesh_path, const std::string& skeleton_path, const std::string& animation_path, int instance_count) {
    mesh_.load(mesh_path);
    //Skeleton skeleton;
    //skeleton.load(skeleton_path);
    //for (int i = 0; i <instance_count ; i++) {
    //    skeletons_.push_back(skeleton);
    //}
    skeletons_.load(skeleton_path);
    animation_set_.load(animation_path);
    instanse_count_ = instance_count;
}
// 指定アニメーション・フレームのローカル行列を計算する
void InstanceSkinnedMesh::compute_local_matrices(int instance_id, int anim_id, float frame_no, Matrix4x4 local[]) const {
    skeletons_/*[instance_id]*/.compute_local_matrices(animation_set_.get(anim_id), frame_no, local);
}
// ワールド行列を計算する
void InstanceSkinnedMesh::compute_world_matrices(int instance_id,const Matrix4x4 local[], Matrix4x4 world[], const Matrix4x4& parent) const {
    skeletons_/*[instance_id]*/.compute_world_matrices(parent, local, world);
}
// スキニング行列を計算する
void InstanceSkinnedMesh::compute_skinned_matrices(int instance_id,const Matrix4x4 world[], Matrix4x4 skinned[]) const {
    skeletons_/*[instance_id]*/.compute_skinned_matrices(world, skinned);
}
// 2つのアニメーションをブレンドしたローカル行列を計算する
void InstanceSkinnedMesh::compute_blended_matrices(int instance_id, int anim_id1, float frame1, int anim_id2, float frame2, float weight, Matrix4x4 local[]) const {
    skeletons_/*[instance_id]*/.compute_blended_matrices(animation_set_.get(anim_id1), frame1,animation_set_.get(anim_id2), frame2,weight, local);
}
// スキニング行列をセットする
void InstanceSkinnedMesh::set_skinned_matrices(const SkinnedMat skinned[], unsigned int instance_count) {
    instance_count = Mathf::CLAMP(instance_count,0,instanse_count_);
    //データの大きさ
    size_t bone = bone_count(0);
    unsigned int size = (unsigned int)bone * instance_count;
    //サイズの変更
    if (instance_count != instnace_skinned_mat_count_) {
        delete instnace_skinned_mat_;
        instnace_skinned_mat_ = new Matrix4x4[size];
        instnace_skinned_mat_count_ = instance_count;
    }
    //データの移し替え
    int current = 0;
    for (unsigned int i = 0; i < instance_count; i++) {
        Matrix4x4* first = skinned[i].skinned;
        for (size_t j = 0; j < bone; j++) {
            instnace_skinned_mat_[current] = first[j];
            current++;
        }
    }
    device_->set_instance_structured_buffer(1, instnace_skinned_mat_, size);
}
// 描画する
void InstanceSkinnedMesh::draw(unsigned int instance_count,unsigned int instance_num_offset) const {
    instance_count = Mathf::CLAMP(instance_count, 0, instanse_count_);
    // メッシュ描画
    mesh_.draw(instance_count,instance_num_offset);
}
// ボーン数を返す
std::size_t InstanceSkinnedMesh::bone_count(int instance_id) const {
    return skeletons_/*[instance_id]*/.bone_count();
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