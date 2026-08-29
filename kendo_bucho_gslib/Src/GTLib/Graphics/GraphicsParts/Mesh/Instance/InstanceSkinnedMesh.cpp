#include "InstanceSkinnedMesh.h"
#include "GTLib/Graphics/GraphicsParts/GraphicsDevice.h"

// コンストラクタ
InstanceSkinnedMesh::InstanceSkinnedMesh(GraphicsDevice* device) :device_{ device }{}
// デストラクタ
InstanceSkinnedMesh::~InstanceSkinnedMesh() {
    clear();
}
//データを追加する
void InstanceSkinnedMesh::add_instance_data(const Matrix4x4& world_matrix, const Matrix4x4 skinned_matrix[], unsigned int bone_num, Color color){
    if (skinned_matrix == nullptr || bone_num == 0)return;
    //インスタンスデータに変換し保存
    InstanceObject obj;
    obj.world_ = world_matrix;
    obj.bone_offset_ = static_cast<unsigned int>(skinned_matrices_.size());
    obj.color[0]= color.r;
    obj.color[1] = color.g;
    obj.color[2] = color.b;
    obj.color[3] = color.a;
    instance_objects_.push_back(obj);
    //ボーンデータを並列化して保存
    for (unsigned int i = 0; i < bone_num; i++ ) {
        skinned_matrices_.push_back(skinned_matrix[i]);
    }
}
//データを初期化する
void InstanceSkinnedMesh::ini_instance_data() {
    instance_objects_.clear();
    skinned_matrices_.clear();
}
// スキニング行列をセットする
void InstanceSkinnedMesh::set_skinned_matrices() {
    device_->set_instance_structured_buffer(1,skinned_matrices_.data(),static_cast<UINT>(skinned_matrices_.size())*sizeof(Matrix4x4));
}
// インスタンスデータをセットする
void InstanceSkinnedMesh::set_instance_matrices() {
    device_->set_instance_structured_buffer(0, instance_objects_.data(), static_cast<UINT>(instance_objects_.size()) * sizeof(InstanceObject));
}
// 消去
void InstanceSkinnedMesh::clear() {
    ini_instance_data();
}
//保存されているインスタンスデータの数を返す
unsigned int InstanceSkinnedMesh::instance_count()const{
    return static_cast<unsigned int>(instance_objects_.size());
}