#include "SkinnedMesh.h"
#include "GTLib/Graphics/GraphicsParts/GraphicsDevice.h"

// コンストラクタ
SkinnedMesh::SkinnedMesh(GraphicsDevice* device) :
    device_{ device }, mesh_{ device } {
}
// デストラクタ
SkinnedMesh::~SkinnedMesh() {
    clear();
}
// ファイル読み込み
void SkinnedMesh::load(const std::string& mesh_path, const std::string& skeleton_path, const std::string& animation_path) {
    mesh_.load(mesh_path);
    skeleton_.load(skeleton_path);
    animation_set_.load(animation_path);
}
// 指定アニメーション・フレームのローカル行列を計算する
void SkinnedMesh::compute_local_matrices(int anim_id, float frame_no, Matrix4x4 local[]) const {
    skeleton_.compute_local_matrices(animation_set_.get(anim_id), frame_no, local);
}
// ワールド行列を計算する
void SkinnedMesh::compute_world_matrices(
    const Matrix4x4 local[], Matrix4x4 world[], const Matrix4x4& parent) const {
    skeleton_.compute_world_matrices(parent, local, world);
}
// スキニング行列を計算する
void SkinnedMesh::compute_skinned_matrices(
    const Matrix4x4 world[], Matrix4x4 skinned[]) const {
    skeleton_.compute_skinned_matrices(world, skinned);
}
// 2つのアニメーションをブレンドしたローカル行列を計算する
void SkinnedMesh::compute_blended_matrices(int anim_id1, float frame1, int anim_id2, float frame2, float weight, Matrix4x4 local[]) const {
    skeleton_.compute_blended_matrices(animation_set_.get(anim_id1), frame1,
        animation_set_.get(anim_id2), frame2,
        weight, local);
}
// スキニング行列をセットする
void SkinnedMesh::set_skinned_matrices(const Matrix4x4 skinned[]) const {
    device_->set_constant_buffer(4, skinned, sizeof(Matrix4x4) * bone_count());
}
// 描画する
void SkinnedMesh::draw() const {
    // メッシュ描画
    mesh_.draw();
}
// ボーン数を返す
std::size_t SkinnedMesh::bone_count() const {
    return skeleton_.bone_count();
}
// 指定アニメーションの終了フレームを返す
float SkinnedMesh::last_frame(int anim_id) const {
    return animation_set_.get(anim_id).last_frame();
}
// 消去
void SkinnedMesh::clear() {
    mesh_.clear();
    skeleton_.clear();
    animation_set_.clear();
}