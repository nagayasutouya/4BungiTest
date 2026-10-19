#ifndef SPHERE_H_
#define SPHERE_H_

#include "GTLib/Math/Vector3.h"

// 球構造体
struct Sphere {
    // 球の中心座標
    Vector3 center{};
    // 球の半径
    float radius{ 0.0f };
    // デフォルトコンストラクタ
    Sphere() = default;
    // コンストラクタ
    Sphere(const Vector3& c, float r);
};

// 球判定結果構造体
struct SphereHit {
    bool hit{ false };// 衝突したか
    Vector3 push_out;// 衝突解消のための押し出しベクトル
    Vector3 point;// 最も深い衝突点
    Vector3 normal;// 衝突面の法線
};

#endif