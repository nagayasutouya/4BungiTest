#include "Quaternion.h"
#include "Vector3.h"
#include "Mathf.h"
#include <cmath>

//内積誤差補正
const float kEpsilon{ 1e-05f };
inline bool is_equal_using_dot(float dot);

// コンストラクタ
Quaternion::Quaternion(float x, float y, float z, float w)
    : x{ x }, y{ y }, z{ z }, w{ w } {
}
// 逆クォータニオン
Quaternion Quaternion::inverse() const {
    float n = x * x + y * y + z * z + w * w;
    if (n <= 0.000001f) return Quaternion{ 0.0f, 0.0f, 0.0f, 1.0f };
    return Quaternion{ -x / n, -y / n, -z / n, w / n };
}
// オイラー角から生成
Quaternion Quaternion::euler(const Vector3& eulers) {
	return euler(eulers.x, eulers.y, eulers.z);
}

// オイラー角から生成
Quaternion Quaternion::euler(float x, float y, float z) {
	Vector3 v = Vector3{ Mathf::deg_to_rad(x), Mathf::deg_to_rad(y), Mathf::deg_to_rad(z) };
	float c1 = std::cos(v.x * 0.5f);
	float c2 = std::cos(v.y * 0.5f);
	float c3 = std::cos(v.z * 0.5f);
	float s1 = std::sin(v.x * 0.5f);
	float s2 = std::sin(v.y * 0.5f);
	float s3 = std::sin(v.z * 0.5f);
	float qx = s1 * c2 * c3 + c1 * s2 * s3;
	float qy = c1 * s2 * c3 - s1 * c2 * s3;
	float qz = c1 * c2 * s3 - s1 * s2 * c3;
	float qw = c1 * c2 * c3 + s1 * s2 * s3;
	return Quaternion{ qx, qy, qz, qw };
}
// 2つの回転aとb間の角度
float Quaternion::angle(const Quaternion& a, const Quaternion& b) {
    float dot = Quaternion::dot(a, b);
    return is_equal_using_dot(dot) ? 0.0f : Mathf::rad_to_deg(std::acos(Mathf::Min(std::abs(dot), 1.0f)) * 2.0f);
}
// 軸と角度からの生成
Quaternion Quaternion::angle_axis(float angle, const Vector3& axis) {
	Vector3 n = axis.normalized();
	float rad = Mathf::deg_to_rad(angle * 0.5f);
	float s = std::sin(rad);
	return Quaternion(n.x * s, n.y * s, n.z * s, std::cos(rad));
}
// 前方ベクトルと上ベクトルからの生成
Quaternion Quaternion::look_rotation(const Vector3& forward, const Vector3& up) {
    // 前方ベクトルを正規化
    Vector3 f = forward.normalized();
    // 右ベクトルを計算
    Vector3 r = Vector3::cross(up, f);
    // 右ベクトルがほぼゼロの場合（真上や真下を向いている場合）の対処
    if (r.sqr_magnitude() < 0.000001f) {
        r = Vector3::cross(Vector3{ 0.0f, 1.0f, 0.0f }, f);
        if (r.sqr_magnitude() < 0.000001f) {
            r = Vector3::cross(Vector3{ 0.0f, 0.0f, 1.0f }, f);
        }
    }
    r = r.normalized();
    Vector3 u = Vector3::cross(f, r);
    u = u.normalized();

    // 回転行列の各成分
    float m00 = r.x, m01 = u.x, m02 = f.x;
    float m10 = r.y, m11 = u.y, m12 = f.y;
    float m20 = r.z, m21 = u.z, m22 = f.z;

    // 行列からクォータニオンへの変換アルゴリズム
    float num8 = (m00 + m11) + m22;
    Quaternion q;
    if (num8 > 0.0f) {
        float num = std::sqrt(num8 + 1.0f);
        q.w = num * 0.5f;
        num = 0.5f / num;
        q.x = (m21 - m12) * num;
        q.y = (m02 - m20) * num;
        q.z = (m10 - m01) * num;
        return q;
    }
    if ((m00 >= m11) && (m00 >= m22)) {
        float num7 = std::sqrt(((1.0f + m00) - m11) - m22);
        float num4 = 0.5f / num7;
        q.x = 0.5f * num7;
        q.y = (m01 + m10) * num4;
        q.z = (m02 + m20) * num4;
        q.w = (m21 - m12) * num4;
        return q;
    }
    if (m11 > m22) {
        float num6 = std::sqrt(((1.0f + m11) - m00) - m22);
        float num3 = 0.5f / num6;
        q.x = (m10 + m01) * num3;
        q.y = 0.5f * num6;
        q.z = (m21 + m12) * num3;
        q.w = (m02 - m20) * num3;
        return q;
    }
    float num5 = std::sqrt(((1.0f + m22) - m00) - m11);
    float num2 = 0.5f / num5;
    q.x = (m20 + m02) * num2;
    q.y = (m21 + m12) * num2;
    q.z = 0.5f * num5;
    q.w = (m10 - m01) * num2;
    return q;
}
// fromからtoへの回転クォータニオンを生成
Quaternion Quaternion::from_to_rotation(const Vector3& from, const Vector3& to) {
    Vector3 f = from.normalized();
    Vector3 t = to.normalized();
    float d = Vector3::dot(f, t);
    // ほぼ同じ方向の場合は単位クォータニオンを返す
    if (d >= 1.0f - 0.000001f) {
        return Quaternion{ 0.0f, 0.0f, 0.0f, 1.0f };
    }
    // ほぼ逆方向の場合（180度回転）
    if (d <= -1.0f + 0.000001f) {
        // 適当な直交軸を求める
        Vector3 axis = Vector3::cross(Vector3{ 0.0f, 1.0f, 0.0f }, f);
        if (axis.sqr_magnitude() < 0.000001f) {
            axis = Vector3::cross(Vector3{ 0.0f, 0.0f, 1.0f }, f);
        }
        axis = axis.normalized();
        // 180度回転（半角90度: sin=1, cos=0）
        return Quaternion{ axis.x, axis.y, axis.z, 0.0f };
    }
    // 通常の場合: 外積で回転軸、内積で角度
    Vector3 c = Vector3::cross(f, t);
    float s = std::sqrt((1.0f + d) * 2.0f);
    float inv_s = 1.0f / s;
    return Quaternion{ c.x * inv_s, c.y * inv_s, c.z * inv_s, s * 0.5f };
}
// fromからtoへの回転クォータニオンを生成
Quaternion Quaternion::rotate_towards(const Quaternion& from, const Quaternion& to, float max_degrees_delta) {
    float angle = Quaternion::angle(from, to);
    if (angle == 0.0f) return to;
    return slerp(from, to, Mathf:: Min(1.0f, max_degrees_delta / angle));
}
// 球面線形補間
Quaternion Quaternion::slerp(const Quaternion& a, const Quaternion& b, float t) {
    float cos_theta = Quaternion::dot(a, b);

    // 最短経路を確保するため、内積が負なら片方を反転
    Quaternion end = (cos_theta < 0.0f) ? -b : b;
    cos_theta = std::abs(cos_theta);

    float scale0, scale1;
    if (1.0f - cos_theta > 0.001f) {
        // 通常の球面補間
        float theta = std::acos(cos_theta);
        float sin_theta = std::sin(theta);
        scale0 = std::sin((1.0f - t) * theta) / sin_theta;
        scale1 = std::sin(t * theta) / sin_theta;
    }
    else {
        // 角が1に近い場合は線形補間(lerp)で近似
        scale0 = 1.0f - t;
        scale1 = t;
    }
    return (a * scale0 + end * scale1).normalized();
}
// 正規化されたクォータニオンを返す
Quaternion Quaternion::normalized() const {
    Quaternion q = *this;
    float mag = std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
    if (mag < 0.000001f) {
        return Quaternion{ 0.0f, 0.0f, 0.0f, 1.0f };
    }
    return Quaternion{ q.x / mag, q.y / mag, q.z / mag, q.w / mag };
}

// 内積
float Quaternion::dot(const Quaternion& lhs, const Quaternion& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}
// 単項マイナス
Quaternion operator - (const Quaternion& lhs) {
    return Quaternion(-lhs.x, -lhs.y, -lhs.z, -lhs.w);
}

// 加算
Quaternion operator + (const Quaternion& lhs, const Quaternion& rhs) {
    return Quaternion(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

// スカラー倍
Quaternion operator * (const Quaternion& lhs, float rhs) {
    return Quaternion(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
}

Quaternion operator * (float lhs, const Quaternion& rhs) {
    return rhs * lhs;
}

// クォータニオン同士の積（回転の合成）
Quaternion operator * (const Quaternion& lhs, const Quaternion& rhs) {
    return Quaternion(
        lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x,
        lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z
    );
}
// ベクトルとの掛け算（クォータニオンによるベクトルの回転）
Vector3 operator * (const Quaternion& rotation, const Vector3& point) {
    float x = rotation.x * 2.0f;
    float y = rotation.y * 2.0f;
    float z = rotation.z * 2.0f;
    float xx = rotation.x * x;
    float yy = rotation.y * y;
    float zz = rotation.z * z;
    float xy = rotation.x * y;
    float xz = rotation.x * z;
    float yz = rotation.y * z;
    float wx = rotation.w * x;
    float wy = rotation.w * y;
    float wz = rotation.w * z;

    Vector3 res;
    res.x = (1.0f - (yy + zz)) * point.x + (xy - wz) * point.y + (xz + wy) * point.z;
    res.y = (xy + wz) * point.x + (1.0f - (xx + zz)) * point.y + (yz - wx) * point.z;
    res.z = (xz - wy) * point.x + (yz + wx) * point.y + (1.0f - (xx + yy)) * point.z;
    return res;
}

// オイラー角の取得
Vector3 Quaternion::euler_angles() const {
    Vector3 euler;
    float sinp = 2.0f * (w * y - z * x);
    if (std::abs(sinp) >= 1.0f) {
        euler.y = std::copysign(Mathf::PI / 2.0f, sinp);
    }
    else {
        euler.y = std::asin(sinp);
    }
    float sinr_cosp = 2.0f * (w * x + y * z);
    float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
    euler.x = std::atan2(sinr_cosp, cosr_cosp);

    float siny_cosp = 2.0f * (w * z + x * y);
    float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
    euler.z = std::atan2(siny_cosp, cosy_cosp);

    return Vector3(
        Mathf::rad_to_deg(euler.x), Mathf::rad_to_deg(euler.y), Mathf::rad_to_deg(euler.z));
}
// 内積を比較する際に誤差を考慮
inline bool is_equal_using_dot(float dot) {
    // Returns false in the presence of NaN values.
    return dot > 1.0f - kEpsilon;
}