#ifndef SKELETON_H_
#define SKELETON_H_

#include <string>
#include <vector>
#include "GTLib/Math/Vector3.h"
#include "GTLib/Math/Matrix4x4.h"

class Animation;

// スケルトンクラス
class Skeleton {
public:
    struct Bone {                               // ボーン構造体
        std::string             name;           // ボーン名
        int                     id;             // ボーンID
        int                     parent_id;      // 親ボーンID
        Vector3                 parent_offset;  // 親ボーンへの相対座標
        Vector3                 position;       // ボーンの絶対座標
    };

public:
    // コンストラクタ
    Skeleton() = default;
    // ファイル読み込み
    void load(const std::string& file_name);
    // ボーン数を返す
    std::size_t bone_count() const;
    // ボーン名を取得
    const std::string& bone_name(int index) const;
    // ボーンのローカル変換行列を計算する
    void compute_local_matrices(const Animation& animation,
        float frame_no, Matrix4x4 local[]) const;
    // ボーンのワールド変換行列を計算する
    void compute_world_matrices(const Matrix4x4& parent,
        const Matrix4x4 local[], Matrix4x4 world[]) const;
    // スキニング用の変換行列を計算する
    void compute_skinned_matrices(const Matrix4x4 world[], Matrix4x4 skinned[]) const;
    // 2つのアニメーションをブレンドした行列を計算する
    void compute_blended_matrices(
        const Animation& anim1, float frame1,
        const Animation& anim2, float frame2,
        float weight,
        Matrix4x4 local[]) const;
    // 消去
    void clear();

private:
    // ボーン配列
    std::vector<Bone> bones_;
};

#endif