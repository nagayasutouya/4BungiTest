#ifndef INSTANCE_ANIMATED_MESH_H_
#define INSTANCE_ANIMATED_MESH_H_

#include "GTLib/GTLib.h"
#include "GTLib/Data/InstanceComputeMatrix.h"
#include <vector>

// アニメーション付きメッシュクラス
class InstanceAnimatedMesh {
public:
    // コンストラクタ
    InstanceAnimatedMesh(unsigned int id,unsigned int instance_count_max, unsigned int motion = 0, bool loop = true, unsigned int num_bones = 256);
    // 更新
    void update(float delta_time);
    // 描画
    void draw() const;
    // モーションの変更
    void change_motion(int instance_id, unsigned int motion, bool loop = true);
    // 変換行列を設定する
    void transform(int instance_id, const Matrix4x4& matrix);
    // モーションが終了しているか？
    bool is_end_motion(int instance_id) const;
    // 現在のモーションの再生時間を取得
    float motion_time(int instance_id) const;
    // モーションの再生時間を設定
    void motion_time(int instance_id, float time);
    // モーションの終了時間を取得
    float motion_end_time(int instance_id) const;
    // ボーンのワールド変換行列を取得
    Matrix4x4 bone_matrices(int instance_id, int bone_no) const;
private:
    unsigned int id_;// アセットID
    unsigned int instance_count_max_{0};//描画の最大数
    unsigned int instance_count_{ 0 };//現在の描画数

    std::vector<InstanceObject> transform_;//全オブジェクトのワールドマトリックスデータ

    std::vector<BlendedMat> local_bone_data_;// ボーンのローカル用データ
    std::vector<WorldMat> bone_matrices_;// ボーンの変換行列
    std::vector<SkinnedMat> skinned_matrices_;// スキニング変換行列
};


#endif