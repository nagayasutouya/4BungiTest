#ifndef AABB_H_
#define AABB_H_

#include "GTLib/Math/Vector3.h"
#include "GTLib/Math/Line.h"
#include "Sphere.h"

// 境界ボックス (AABB)
struct AABB {
    // AABBの最小の点の座標
    Vector3 min_bound{};
    // AABBの最大の点の座標
    Vector3 max_bound{};

    // デフォルトコンストラクタ
    AABB() = default;
    // コンストラクタ
    AABB(const Vector3& min_bound, const Vector3& max_bound);
    // 球との交差判定
    bool intersects_sphere(const Sphere& sphere) const;
    // 線分との交差判定
    bool intersects_line_segment(const Line& line) const;
};

#endif