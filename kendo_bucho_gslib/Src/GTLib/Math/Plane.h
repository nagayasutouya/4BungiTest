#ifndef PLANE_H_
#define PLANE_H_

#include "Vector3.h"

// 平面構造体
struct Plane {
    // 法線ベクトル
    Vector3 normal;
    // 平面方程式 (ax + by + cz + d = 0) のd項
    float d{ 0.0f };

    // デフォルトコンストラクタ
    Plane() = default;
    // コンストラクタ (法線ベクトル, 距離)
    Plane(const Vector3& normal, float d);
    // コンストラクタ (法線a, b, c, 距離)
    Plane(float a, float b, float c, float d);

    // 点と平面の符号付き距離
    float distance_to_point(const Vector3& point) const;
};

#endif
