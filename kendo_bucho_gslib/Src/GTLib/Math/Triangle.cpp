#include "Triangle.h"
#include <cmath>

// コンストラクタ
Triangle::Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Plane& plane) : plane{ plane } {
    vertices[0] = v0;
    vertices[1] = v1;
    vertices[2] = v2;
}
// 点が三角形の内部（面上領域）にあるかどうか（重心座標系による判定）
bool Triangle::contains_point(const Vector3& point) const {
    // 三角形の2辺ベクトルと、頂点0から点へのベクトルを計算
    Vector3 e0 = vertices[2] - vertices[0];
    Vector3 e1 = vertices[1] - vertices[0];
    Vector3 e2 = point - vertices[0];
    // 射影を計算
    float d00 = Vector3::dot(e0, e0);
    float d01 = Vector3::dot(e0, e1);
    float d11 = Vector3::dot(e1, e1);
    float d20 = Vector3::dot(e2, e0);
    float d21 = Vector3::dot(e2, e1);
    // 縮退三角形（面積0）のケースを回避
    float denom = d00 * d11 - d01 * d01;
    if (std::abs(denom) < 1e-8f) return false;
    // 重心座標を計算
    float v_weight = (d11 * d20 - d01 * d21) / denom;
    float w_weight = (d00 * d21 - d01 * d20) / denom;
    float u_weight = 1.0f - v_weight - w_weight;
    // 許容誤差を持たせて境界上の点も内側と判定する
    return (u_weight >= -1e-5f) && (v_weight >= -1e-5f) && (w_weight >= -1e-5f);
}
// 点から三角形への最近接点計算
Vector3 Triangle::closest_point(const Vector3& point) const {
    // 辺AB, AC, APを計算
    Vector3 ab = vertices[1] - vertices[0];
    Vector3 ac = vertices[2] - vertices[0];
    Vector3 ap = point - vertices[0];
    // 射影を計算
    float d1 = Vector3::dot(ab, ap);
    float d2 = Vector3::dot(ac, ap);
    // 頂点Aが最近接点の場合
    if (d1 <= 0.0f && d2 <= 0.0f) return vertices[0];

    // 頂点Bが最近接点の場合
    Vector3 bp = point - vertices[1];
    float d3 = Vector3::dot(ab, bp);
    float d4 = Vector3::dot(ac, bp);
    if (d3 >= 0.0f && d4 <= d3) return vertices[1];

    // 辺AB上が最近接点の場合
    float vc = d1 * d4 - d3 * d2;
    if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
        float v_weight = d1 / (d1 - d3);
        return vertices[0] + ab * v_weight;
    }

    // 頂点Cが最近接点の場合
    Vector3 cp = point - vertices[2];
    float d5 = Vector3::dot(ab, cp);
    float d6 = Vector3::dot(ac, cp);
    if (d6 >= 0.0f && d5 <= d6) return vertices[2];

    // 辺BC上が最近接点の場合
    float vb = d5 * d2 - d1 * d6;
    if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
        float w_weight = d2 / (d2 - d6);
        return vertices[0] + ac * w_weight;
    }

    // 辺BC上が最近接点の場合
    float va = d3 * d6 - d5 * d4;
    // 内部が最近接点の場合
    if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
        float w_weight = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        return vertices[1] + (vertices[2] - vertices[1]) * w_weight;
    }

    // 内部が最近接点の場合
    float inv_denom = 1.0f / (va + vb + vc);
    float v_weight = vb * inv_denom;
    float w_weight = vc * inv_denom;
    return vertices[0] + ab * v_weight + ac * w_weight;
}
// 線分と三角形の交差判定
bool Triangle::intersect_line_segment(const Vector3& seg_start, const Vector3& dir_vec, float max_dist, LineHit& out_hit) const {
    float denominator = Vector3::dot(plane.normal, dir_vec);
    // 線分が面に平行な場合は交差しない
    if (std::abs(denominator) < 1e-6f) return false;
    // 交差するまでの距離を計算
    float t = -plane.distance_to_point(seg_start) / denominator;
    // 線分が交差する点より手前にある場合は除外
    if (t < 0.0f || t > max_dist) return false;
    // 交差点を計算
    Vector3 point = seg_start + dir_vec * t;
    // 交差点が三角形の内部にあるか判定
    if (contains_point(point)) {
        out_hit.hit = true;
        out_hit.point = point;
        out_hit.normal = (denominator < 0.0f) ? plane.normal : -plane.normal;
        out_hit.distance = t;
        return true;
    }
    return false;
}