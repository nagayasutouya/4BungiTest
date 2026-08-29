#ifndef ANIMATED_MESH_H_
#define ANIMATED_MESH_H_

#include"GTLib/GTLib.h"
#include <vector>

// アニメーション付きメッシュクラス
class AnimatedMesh {
public:
    // コンストラクタ
    AnimatedMesh(unsigned int id, unsigned int motion = 0, bool loop = true, unsigned int num_bones = 256);
    // 更新
    void update(float delta_time);
    // 描画
    void draw() const;
    // モーションの変更
    void change_motion(unsigned int motion, bool loop = true);
    // 変換行列を設定する
    void transform(const Matrix4x4& matrix);
    // モーションが終了しているか？
    bool is_end_motion() const;
    // 現在のモーションの再生時間を取得
    float motion_time() const;
    // モーションの再生時間を設定
    void motion_time(float time);
    // モーションの終了時間を取得
    float motion_end_time() const;
    // ボーンのワールド変換行列を取得
    Matrix4x4 bone_matrices(int bone_no) const;
    //スキニング行列の先頭アドレスを取得
    const Matrix4x4* skinned_matrix()const;
private:
    // アセットID
    unsigned int      id_;
    // モーション番号
    unsigned int      motion_;
    // モーションタイマ
    float     motion_timer_;
    // モーションループフラグ
    bool        motion_loop_;
    // ワールド変換行列
    Matrix4x4   transform_;
    // 前回再生したモーション番号
    unsigned int      prev_motion_;
    // 前回再生した最終アニメーションタイマ
    float     prev_motion_timer_;
    // 補間タイマ
    float     lerp_timer_;

    // ボーンのローカル用変換行列
    std::vector<Matrix4x4> local_bone_matrices_;
    // ボーンの変換行列
    std::vector<Matrix4x4> bone_matrices_;
    // スキニング変換行列
    std::vector<Matrix4x4> skinned_matrices_;
};



#endif

