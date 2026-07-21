#include "Vector3.h"
#include "Mathf.h"
#include <cmath>
// コンストラクタ
Vector3::Vector3(float x, float y, float z) :
    x{ x }, y{ y }, z{ z } {
}
// 長さ
float Vector3::magnitude() const {
    return std::sqrt(sqr_magnitude());
}

// 長さの2乗
float Vector3::sqr_magnitude() const {
    return x * x + y * y + z * z;
}
// 正規化されたベクトルを返す
Vector3 Vector3::normalized() const {
    const float mag = magnitude();
    if (mag < 0.00001f) { // ほぼ0ベクトルか？
        return Vector3{ 0.0f, 0.0f, 0.0f };
    }
    return Vector3{ x / mag, y / mag, z / mag };
}
// 内積
float Vector3::dot(const Vector3& lhs, const Vector3& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
// 外積
Vector3 Vector3::cross(const Vector3& lhs, const Vector3& rhs) {
    return Vector3{
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    };
}
// ピッチとヨーからベクトル生成
Vector3 Vector3::create_from_pitch_yaw(float pitch, float yaw) {
    const float rad_pitch = Mathf::deg_to_rad(pitch);
    const float rad_yaw = Mathf::deg_to_rad(yaw);
    return Vector3{
        std::cos(rad_pitch) * std::sin(rad_yaw),
       -std::sin(rad_pitch),
        std::cos(rad_pitch) * std::cos(rad_yaw)
    };
}
// 線形補間
Vector3 Vector3::lerp(const Vector3& a, const Vector3& b, float t) {
    return a + (b - a) * t;
}
// 演算子オーバーロード (単項マイナス)
Vector3 operator - (const Vector3& v) {
    return Vector3{ -v.x, -v.y, -v.z };
}


// 演算子オーバーロード (加算)
Vector3 operator + (const Vector3& lhs, const Vector3& rhs) {
    return Vector3{ lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

// 演算子オーバーロード (減算)
Vector3 operator - (const Vector3& lhs, const Vector3& rhs) {
    return Vector3{ lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}

// 演算子オーバーロード (スカラー倍)
Vector3 operator * (const Vector3& lhs, float rhs) {
    return Vector3{ lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
}

// 演算子オーバーロード (スカラー倍)
Vector3 operator * (float lhs, const Vector3& rhs) {
    return rhs * lhs;
}

// 演算子オーバーロード (除算)
Vector3 operator / (const Vector3& lhs, float rhs) {
    return Vector3{ lhs.x / rhs, lhs.y / rhs, lhs.z / rhs };
}

// 代入加算
Vector3& operator += (Vector3& lhs, const Vector3& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

// 代入減算
Vector3& operator -= (Vector3& lhs, const Vector3& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
}

// 代入乗算
Vector3& operator *= (Vector3& lhs, float rhs) {
    lhs.x *= rhs;
    lhs.y *= rhs;
    lhs.z *= rhs;
    return lhs;
}

// 代入除算
Vector3& operator /= (Vector3& lhs, float rhs) {
    lhs.x /= rhs;
    lhs.y /= rhs;
    lhs.z /= rhs;
    return lhs;
}

