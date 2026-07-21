#include "Vector2.h"
#include <cmath>
#include <algorithm>

// コンストラクタ
Vector2::Vector2(float x, float y) : x{ x }, y{ y } {
}

// ベクトルの長さ
float Vector2::magnitude() const {
    return std::sqrt(x * x + y * y);
}

// 正規化したベクトルを返す
Vector2 Vector2::normalized() const {
    float mag = magnitude();
    if (mag < 0.00001f) { // ほぼ0ベクトルか？
        return *this;
    }
    return Vector2{ x / mag, y / mag };
}

// 2 つのベクトルの内積
float Vector2::dot(const Vector2& lhs, const Vector2& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

// 最小値と最大値の範囲内にクランプしたベクトルを返す
Vector2 Vector2::clamp(const Vector2& v, const Vector2& min, const Vector2& max) {
    return Vector2{ std::clamp(v.x, min.x, max.x), std::clamp(v.y, min.y, max.y) };
}
// 線形補間
Vector2 Vector2::lerp(const Vector2& a, const Vector2& b, float t) {
    return a + (b - a) * t;
}
Vector2 Vector2::lerp(const Vector2& a, float t) {
    return a + (Vector2{x,y} - a) * t;
}

// 逆ベクトル
Vector2 operator - (const Vector2& v) {
    return Vector2{ -v.x, -v.y };
}

// 加算
Vector2 operator + (const Vector2& lhs, const Vector2& rhs) {
    return Vector2{ lhs.x + rhs.x, lhs.y + rhs.y };
}

// 減算
Vector2 operator - (const Vector2& lhs, const Vector2& rhs) {
    return Vector2{ lhs.x - rhs.x, lhs.y - rhs.y };
}

// スカラー倍 V * k
Vector2 operator * (const Vector2& lhs, float rhs) {
    return Vector2{ lhs.x * rhs, lhs.y * rhs };
}

// スカラー倍 k * V
Vector2 operator * (float lhs, const Vector2& rhs) {
    return Vector2{ lhs * rhs.x, lhs * rhs.y };
}

// スカラーで除算
Vector2 operator / (const Vector2& lhs, float rhs) {
    return Vector2{ lhs.x / rhs, lhs.y / rhs };
}

// 複合代入演算子
Vector2& operator += (Vector2& lhs, const Vector2& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}


Vector2& operator -= (Vector2& lhs, const Vector2& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}


Vector2& operator *= (Vector2& lhs, float rhs) {
    lhs.x *= rhs;
    lhs.y *= rhs;
    return lhs;
}


Vector2& operator /= (Vector2& lhs, float rhs) {
    lhs.x /= rhs;
    lhs.y /= rhs;
    return lhs;
}