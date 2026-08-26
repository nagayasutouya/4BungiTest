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
    Matrix4x4* world = nullptr;
    Matrix4x4 parent = Matrix4x4::identity();
};
//スキニング行列
struct SkinnedMat {
    Matrix4x4* skinned = nullptr;
};
//スキニングとワールドマトリックスをブレンド
struct BlendedMat {
    bool is_empty = true;//このIDを使ってるオブジェクトの有無
    int prev_motion = 0;
    float prev_frame = 0.0f;
    int current_motion = 0;
    float current_frame = 0.0f;
    float lerp = 0.0f;
    bool loop = false;
    Matrix4x4* local = nullptr;
};
//HLSL用インスタンススキニングデータ
struct InstanceObject {
    Matrix4x4 world_ = Matrix4x4::identity();
    unsigned int bone_offset_=0;
    unsigned int padding[3]{};//きりよくするための穴埋め
};

#endif