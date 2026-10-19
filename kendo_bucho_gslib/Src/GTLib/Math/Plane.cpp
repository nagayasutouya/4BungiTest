#include "Plane.h"

// コンストラクタ (法線ベクトル, 距離)
Plane::Plane(const Vector3& normal, float d) :
    normal{ normal }, d{ d } {
}
// コンストラクタ (法線a, b, c, 距離)
Plane::Plane(float a, float b, float c, float d) :
    Plane{ Vector3{ a, b, c }, d } {
}
// 点と平面の符号付き距離
float Plane::distance_to_point(const Vector3& point) const {
    return Vector3::dot(normal, point) + d;
}