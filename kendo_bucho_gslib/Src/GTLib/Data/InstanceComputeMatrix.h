#ifndef INSTANCE_COMPUTE_MATRIX_H_
#define INSTANCE_COMPUTE_MATRIX_H_

#include "GTLib/Math/Matrix4x4.h"

//アニメーションのローカルマトリックス計算
struct LocalMat {
    int anim_id = 0;
    float frame_no = 0.0f;
    Matrix4x4* local = nullptr;
};
//ローカルマトリックスをワールドマトリックスに
struct WorldMat {
    Matrix4x4* local = nullptr;
    Matrix4x4* world = nullptr;
    Matrix4x4 parent = Matrix4x4::identity();
};
//スキニング行列
struct SkinnedMat {
    const Matrix4x4* world = nullptr;
    Matrix4x4* skinned = nullptr;
};
//スキニングとワールドマトリックスをブレンド
struct BlendedMat {
    int anim_id1 = 0;
    float frame1 = 0.0f;
    int anim_id2 = 0;
    float frame2 = 0.0f;
    float weight = 0.0f;
    Matrix4x4* local = nullptr;
};
//HLSL用インスタンススキニングデータ
struct InstanceObject {
    Matrix4x4 world_;
    unsigned int bone_offset_;
};

#endif