#ifndef INSTANCE_SKINNED_MESH_H_
#define INSTANCE_SKINNED_MESH_H_

#include <string>
#include "GTLib/Graphics/GraphicsParts/d3dx12.h"
#include "GTLib/Math/Mathf.h"
#include "GTLib/Graphics/GraphicsParts/Mesh/Mesh.h"
#include "GTLib/Graphics/GraphicsParts/Mesh/Skeleton.h"
#include "GTLib/Graphics/GraphicsParts/Mesh/AnimationSet.h"
#include "GTLib/Data/InstanceComputeMatrix.h"
#include <vector>

class GraphicsDevice;

// インスタンス描画用スキンドメッシュクラス
class InstanceSkinnedMesh {
public:
    // コンストラクタ
    InstanceSkinnedMesh(GraphicsDevice* device);
    // デストラクタ
    ~InstanceSkinnedMesh();

    // ファイル読み込み
    void load(const std::string& mesh_path,const std::string& skeleton_path,const std::string& animation_path,int instance_count);

    // 指定アニメーション・フレームのローカル行列を計算する
    void compute_local_matrices(int instance_id,int anim_id, float frame_no, Matrix4x4 local[]) const;
    // ワールド行列を計算する
    void compute_world_matrices(int instance_id,const Matrix4x4 local[], Matrix4x4 world[], const Matrix4x4& parent = Matrix4x4::identity()) const;
    // スキニング行列を計算する
    void compute_skinned_matrices(int instance_id, const Matrix4x4 world[], Matrix4x4 skinned[]) const;
    // 2つのアニメーションをブレンドした行列を計算する
    void compute_blended_matrices(int instance_id, int anim_id1, float frame1,int anim_id2, float frame2,float weight, Matrix4x4 local[]) const;

    //描画数を変更する
    void set_current_instance_count(UINT current) { current_instance_count_ = current; }
    // スキニング行列をセットする
    void set_skinned_matrices(const SkinnedMat skinned[], unsigned int instance_count);
    // 描画する
    void draw(unsigned int instance_count, unsigned int instance_num_offset=0) const;
    
    // ボーン数を返す
    std::size_t bone_count(int instance_id) const;
    // 指定アニメーションの終了フレームを返す
    float last_frame(int anim_id) const;

    // 消去
    void clear();

    // コピー禁止
    InstanceSkinnedMesh(const InstanceSkinnedMesh&) = delete;
    InstanceSkinnedMesh& operator=(const InstanceSkinnedMesh&) = delete;

private:
    // グラフィックスデバイス
    GraphicsDevice* device_{ nullptr };
    //コマンドリスト
    ID3D12GraphicsCommandList* command_list_{nullptr};
    // メッシュ
    Mesh mesh_;
    // スケルトン
    //std::vector<Skeleton> skeletons_;
    Skeleton skeletons_;
    // アニメーション
    AnimationSet animation_set_;
    //描画の最大数
    int instanse_count_{ 0 };
    //今描画する数
    UINT current_instance_count_{ 0 };

    //インスタンス描画用スキニング行列の先頭アドレス
    Matrix4x4* instnace_skinned_mat_{ nullptr };
    //インスタンス描画用スキニング行列の先頭アドレス
    unsigned int instnace_skinned_mat_count_{ 0 };
};

#endif