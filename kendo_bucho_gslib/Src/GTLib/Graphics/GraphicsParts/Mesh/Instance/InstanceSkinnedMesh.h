#ifndef INSTANCE_SKINNED_MESH_H_
#define INSTANCE_SKINNED_MESH_H_

#include <string>
#include "GTLib/Graphics/GraphicsParts/d3dx12.h"
#include "GTLib/Math/Mathf.h"
#include "GTLib/Data/InstanceComputeMatrix.h"
#include "GTLib/Math/Color.h"
#include <vector>

class GraphicsDevice;

// インスタンス描画用スキンドメッシュクラス
class InstanceSkinnedMesh {
public:
    // コンストラクタ
    InstanceSkinnedMesh(GraphicsDevice* device);
    // デストラクタ
    ~InstanceSkinnedMesh();

    //データを追加する
    void add_instance_data(const Matrix4x4& world_matrix,const Matrix4x4 skinned_matrix[],unsigned int bone_num,Color color);
    //データを初期化する
    void ini_instance_data();
    // スキニング行列をセットする
    void set_skinned_matrices();
    // インスタンスデータをセットする
    void set_instance_matrices();

    // 消去
    void clear();

    //保存されているインスタンスデータの数を返す
    unsigned int instance_count()const;

    // コピー禁止
    InstanceSkinnedMesh(const InstanceSkinnedMesh&) = delete;
    InstanceSkinnedMesh& operator=(const InstanceSkinnedMesh&) = delete;

private:
    // グラフィックスデバイス
    GraphicsDevice* device_{ nullptr };
    
    //インスタンスデータ
    std::vector<InstanceObject> instance_objects_;
    //スキニングデータ
    std::vector<Matrix4x4> skinned_matrices_;
};

#endif