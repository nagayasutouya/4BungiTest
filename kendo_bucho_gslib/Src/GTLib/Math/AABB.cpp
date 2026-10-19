#include "AABB.h"
#include <cmath>
#include <algorithm>

// コンストラクタ
AABB::AABB(const Vector3& min_bound, const Vector3& max_bound) :min_bound{ min_bound }, max_bound{ max_bound }{}
// 球との交差判定
bool AABB::intersects_sphere(const Sphere& sphere) const {
    // AABB上の最近接点 (Closest Point) を求める
    Vector3 closest{
        std::clamp(sphere.center.x, min_bound.x, max_bound.x),
        std::clamp(sphere.center.y, min_bound.y, max_bound.y),
        std::clamp(sphere.center.z, min_bound.z, max_bound.z)
    };
    // 球の中心と最近接点との距離の2乗が、半径の2乗以下なら衝突している
    return Vector3::sqr_distance(sphere.center, closest) <= (sphere.radius * sphere.radius);
}
// 1軸分のスラブ（平行平面）による線分クリッピング（スラブ法）
static bool clip_axis(float origin, float dir, float min_val, float max_val,
    float& tmin, float& tmax) {
    // 線分がこの軸に平行（移動量がほぼ 0）な場合
    if (std::abs(dir) < 1e-6f) {
        // 始点がスラブの範囲外にあれば、絶対に交差しない
        return (origin >= min_val && origin <= max_val);
    }
    // 2枚の境界平面（min_val, max_val）と交差する位置パラメータ t1, t2 を計算
    // t = (境界値 - 始点) / 移動量
    float ood = 1.0f / dir;
    float t1 = (min_val - origin) * ood;
    float t2 = (max_val - origin) * ood;
    // 進行方向が負の場合は交差順序が逆転するため、手前(t1) と 奥(t2) を揃える
    if (t1 > t2) std::swap(t1, t2);
    // 「進入位置(tmin)」は最も大きい値、「脱出位置(tmax)」は最も小さい値へと区間を絞り込む
    tmin = std::max(tmin, t1);
    tmax = std::min(tmax, t2);
    // 有効な交差区間が残っているか判定（進入位置が脱出位置より手前なら交差可能）
    return tmin <= tmax;
}
// 線分との交差判定
bool AABB::intersects_line_segment(const Line& line) const {
    Vector3 dir = line.end - line.start;
    float tmin = 0.0f;
    float tmax = 1.0f;
    // 軸ごとにクリッピング
    if (!clip_axis(line.start.x, dir.x, min_bound.x, max_bound.x, tmin, tmax)) return false;
    if (!clip_axis(line.start.y, dir.y, min_bound.y, max_bound.y, tmin, tmax)) return false;
    if (!clip_axis(line.start.z, dir.z, min_bound.z, max_bound.z, tmin, tmax)) return false;
    return true;
}