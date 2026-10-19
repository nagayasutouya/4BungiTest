#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "Vector3.h"
#include "Plane.h"
#include "Line.h"

// 三角形コライダ
struct Triangle {
    // 頂点
    Vector3 vertices[3];
    // 平面
    Plane plane;

    // デフォルトコンストラクタ
    Triangle() = default;
    // コンストラクタ
    Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Plane& plane);

    // 点が三角形の内部（面上領域）にあるかどうか（重心座標系による判定）
    bool contains_point(const Vector3& point) const;
    // 点から三角形への最近接点計算
    Vector3 closest_point(const Vector3& point) const;
    // 線分と三角形の交差判定
    bool intersect_line_segment(const Vector3& seg_start, const Vector3& dir_vec,
        float max_dist, LineHit& out_hit) const;
};

#endif
