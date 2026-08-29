#ifndef SKINNED_MESH_H_
#define SKINNED_MESH_H_

#include <string>
#include "Mesh.h"
#include "Skeleton.h"
#include "AnimationSet.h"
#include "GTLib/Math/Matrix4x4.h"

class GraphicsDevice;

// スキンドメッシュクラス
class SkinnedMesh {
public:
    // コンストラクタ
    SkinnedMesh(GraphicsDevice* device);
    // デストラクタ
    ~SkinnedMesh();

    // ファイル読み込み
    void load(
        const std::string& mesh_path,
        const std::string& skeleton_path,
        const std::string& animation_path);

    // 指定アニメーション・フレームのローカル行列を計算する
    void compute_local_matrices(int anim_id, float frame_no, Matrix4x4 local[]) const;
    // ワールド行列を計算する
    void compute_world_matrices(const Matrix4x4 local[], Matrix4x4 world[],
        const Matrix4x4& parent = Matrix4x4::identity()) const;
    // スキニング行列を計算する
    void compute_skinned_matrices(const Matrix4x4 world[], Matrix4x4 skinned[]) const;
    // 2つのアニメーションをブレンドした行列を計算する
    void compute_blended_matrices(
        int anim_id1, float frame1,
        int anim_id2, float frame2,
        float weight, Matrix4x4 local[]) const;

    // スキニング行列をセットする
    void set_skinned_matrices(const Matrix4x4 skinned[]) const;
    // 描画する
    void draw(unsigned int instance_count = 1, unsigned int instance_num_offset = 0, unsigned int start_vartex = 0) const;

    // ボーン数を返す
    std::size_t bone_count() const;
    // 指定アニメーションの終了フレームを返す
    float last_frame(int anim_id) const;

    // 消去
    void clear();

    // コピー禁止
    SkinnedMesh(const SkinnedMesh&) = delete;
    SkinnedMesh& operator=(const SkinnedMesh&) = delete;

private:
    // グラフィックスデバイス
    GraphicsDevice* device_{ nullptr };
    // メッシュ
    Mesh mesh_;
    // スケルトン
    Skeleton skeleton_;
    // アニメーション
    AnimationSet animation_set_;
};

#endif

