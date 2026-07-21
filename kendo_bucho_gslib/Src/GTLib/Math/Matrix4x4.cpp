#include "Matrix4x4.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Mathf.h"
#include <cmath>

// コンストラクタ
Matrix4x4::Matrix4x4(
    float m11, float m12, float m13, float m14,
    float m21, float m22, float m23, float m24,
    float m31, float m32, float m33, float m34,
    float m41, float m42, float m43, float m44) {
    m[0][0] = m11; m[0][1] = m12; m[0][2] = m13; m[0][3] = m14;
    m[1][0] = m21; m[1][1] = m22; m[1][2] = m23; m[1][3] = m24;
    m[2][0] = m31; m[2][1] = m32; m[2][2] = m33; m[2][3] = m34;
    m[3][0] = m41; m[3][1] = m42; m[3][2] = m43; m[3][3] = m44;
}
// 零行列
Matrix4x4 Matrix4x4::zero() {
    return Matrix4x4{
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f
    };
}
// 単位行列
Matrix4x4 Matrix4x4::identity() {
    return Matrix4x4{
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}
// 拡大縮小行列
Matrix4x4 Matrix4x4::scale(const Vector3& vector) {
    Matrix4x4 result = identity();
    result.m[0][0] = vector.x;
    result.m[1][1] = vector.y;
    result.m[2][2] = vector.z;
    return result;
}
// x軸回転行列
Matrix4x4 Matrix4x4::rotate_x(float degree) {
    float r = Mathf::deg_to_rad(degree);
    float c = std::cos(r);
    float s = std::sin(r);
    Matrix4x4 result = identity();
    result.m[1][1] = c; result.m[1][2] = s;
    result.m[2][1] = -s; result.m[2][2] = c;
    return result;
}
// y軸回転行列
Matrix4x4 Matrix4x4::rotate_y(float degree) {
    float r = Mathf::deg_to_rad(degree);
    float c = std::cos(r);
    float s = std::sin(r);
    Matrix4x4 result = identity();
    result.m[0][0] = c; result.m[0][2] = -s;
    result.m[2][0] = s; result.m[2][2] = c;
    return result;
}
// z軸回転行列
Matrix4x4 Matrix4x4::rotate_z(float degree) {
    float r = Mathf::deg_to_rad(degree);
    float c = std::cos(r);
    float s = std::sin(r);
    Matrix4x4 result = identity();
    result.m[0][0] = c; result.m[0][1] = s;
    result.m[1][0] = -s; result.m[1][1] = c;
    return result;
}
// 任意軸回転行列
Matrix4x4 Matrix4x4::rotate(float degree, const Vector3& axis) {
    return Matrix4x4::rotate(Quaternion::angle_axis(degree, axis));
}
// クォータニオン回転行列
Matrix4x4 Matrix4x4::rotate(const Quaternion& q) {
    float xx = q.x * q.x;
    float yy = q.y * q.y;
    float zz = q.z * q.z;
    float xy = q.x * q.y;
    float xz = q.x * q.z;
    float yz = q.y * q.z;
    float wx = q.w * q.x;
    float wy = q.w * q.y;
    float wz = q.w * q.z;

    return Matrix4x4{
        1.0f - 2.0f * (yy + zz), 2.0f * (xy + wz), 2.0f * (xz - wy), 0.0f,
        2.0f * (xy - wz), 1.0f - 2.0f * (xx + zz), 2.0f * (yz + wx), 0.0f,
        2.0f * (xz + wy), 2.0f * (yz - wx), 1.0f - 2.0f * (xx + yy), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}
// 平行移動行列
Matrix4x4 Matrix4x4::translate(const Vector3& p) {
    Matrix4x4 result = identity();
    result.m[3][0] = p.x;
    result.m[3][1] = p.y;
    result.m[3][2] = p.z;
    return result;
}
// TRS行列 (並行移動、回転、拡大を合成)
Matrix4x4 Matrix4x4::TRS(const Vector3& p, const Quaternion& q, const Vector3& s) {
    Matrix4x4 result = rotate(q);
    result.m[0][0] *= s.x; result.m[0][1] *= s.x; result.m[0][2] *= s.x;
    result.m[1][0] *= s.y; result.m[1][1] *= s.y; result.m[1][2] *= s.y;
    result.m[2][0] *= s.z; result.m[2][1] *= s.z; result.m[2][2] *= s.z;
    result.m[3][0] = p.x;  result.m[3][1] = p.y;  result.m[3][2] = p.z;
    return result;
}
// 視野変換行列 (右手座標系)
Matrix4x4 Matrix4x4::look_at(const Vector3& eye, const Vector3& at, const Vector3& up) {
    Vector3 z = (eye - at).normalized();
    Vector3 x = Vector3::cross(up, z).normalized();
    Vector3 y = Vector3::cross(z, x);
    return Matrix4x4{
        x.x, y.x, z.x, 0.0f,
        x.y, y.y, z.y, 0.0f,
        x.z, y.z, z.z, 0.0f,
        -Vector3::dot(eye, x), -Vector3::dot(eye, y), -Vector3::dot(eye, z), 1.0f
    };
}
// 透視投影行列 (右手座標系)
Matrix4x4 Matrix4x4::perspective(float fov, float aspect, float zn, float zf) {
    float f = 1.0f / std::tan(Mathf::deg_to_rad(fov / 2.0f));
    return Matrix4x4{
        f / aspect, 0.0f, 0.0f, 0.0f,
        0.0f, f,    0.0f, 0.0f,
        0.0f, 0.0f, zf / (zn - zf), -1.0f,
        0.0f, 0.0f, (zn * zf) / (zn - zf), 0.0f
    };
}
// 座標変換 (w除算あり)
Vector3 Matrix4x4::multiply_point(const Vector3& v) const {
    float x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0];
    float y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1];
    float z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2];
    float w = v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + m[3][3];
    return Vector3{ x / w, y / w, z / w };
}
// 座標変換 (4x3)
Vector3 Matrix4x4::multiply_point4x3(const Vector3& v) const {
    float x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0];
    float y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1];
    float z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2];
    return Vector3{ x, y, z };
}
// ベクトル変換 (平行移動なし)
Vector3 Matrix4x4::multiply_vector(const Vector3& v) const {
    float x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0];
    float y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1];
    float z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2];
    return Vector3{ x, y, z };
}
// スケール値の取得
Vector3 Matrix4x4::scale() const {
    return Vector3{
        Vector3(m[0][0], m[0][1], m[0][2]).magnitude(),
        Vector3(m[1][0], m[1][1], m[1][2]).magnitude(),
        Vector3(m[2][0], m[2][1], m[2][2]).magnitude()
    };
}
// 回転を取得
Quaternion Matrix4x4::rotation() const {
    Quaternion result;
    float tr = m[0][0] + m[1][1] + m[2][2] + 1.0f;
    if (tr >= 1.0f) {
        float fourD = 2.0f * std::sqrt(tr);
        result.x = (m[2][1] - m[1][2]) / fourD;
        result.y = (m[0][2] - m[2][0]) / fourD;
        result.z = (m[1][0] - m[0][1]) / fourD;
        result.w = fourD / 4.0f;
        return result;
    }
    int i = 0;
    if (m[0][0] <= m[1][1]) {
        i = 1;
    }
    if (m[2][2] > m[i][i]) {
        i = 2;
    }
    int j = (i + 1) % 3;
    int k = (j + 1) % 3;
    tr = m[i][i] - m[j][j] - m[k][k] + 1.0f;
    float fourD = 2.0f * std::sqrt(tr);
    float q[4];
    q[i] = fourD / 4.0f;
    q[j] = (m[j][i] + m[i][j]) / fourD;
    q[k] = (m[k][i] + m[i][k]) / fourD;
    q[3] = (m[k][j] - m[j][k]) / fourD;
    return Quaternion{ q[0], q[1], q[2], q[3] };
}
// 前方向ベクトルを取得
Vector3 Matrix4x4::forward() const {
    return Vector3(m[2][0], m[2][1], m[2][2]).normalized();
}
// 上方向ベクトルを取得
Vector3 Matrix4x4::up() const {
    return Vector3(m[1][0], m[1][1], m[1][2]).normalized();
}
// 左方向ベクトルを取得
Vector3 Matrix4x4::left() const {
    return Vector3{ m[0][0], m[0][1], m[0][2] }.normalized();
}
// 位置を取得
Vector3 Matrix4x4::position() const {
    return Vector3{ m[3][0], m[3][1], m[3][2] };
}
// 転置行列
Matrix4x4 Matrix4x4::transpose() const {
    Matrix4x4 result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = m[j][i];
        }
    }
    return result;
}
// 逆行列
Matrix4x4 Matrix4x4::inverse() const {
    float a0 = m[0][0] * m[1][1] - m[0][1] * m[1][0];
    float a1 = m[0][0] * m[1][2] - m[0][2] * m[1][0];
    float a2 = m[0][0] * m[1][3] - m[0][3] * m[1][0];
    float a3 = m[0][1] * m[1][2] - m[0][2] * m[1][1];
    float a4 = m[0][1] * m[1][3] - m[0][3] * m[1][1];
    float a5 = m[0][2] * m[1][3] - m[0][3] * m[1][2];
    float b0 = m[2][0] * m[3][1] - m[2][1] * m[3][0];
    float b1 = m[2][0] * m[3][2] - m[2][2] * m[3][0];
    float b2 = m[2][0] * m[3][3] - m[2][3] * m[3][0];
    float b3 = m[2][1] * m[3][2] - m[2][2] * m[3][1];
    float b4 = m[2][1] * m[3][3] - m[2][3] * m[3][1];
    float b5 = m[2][2] * m[3][3] - m[2][3] * m[3][2];

    float det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
    if (std::abs(det) < 0.0001f) {
        return identity();
    }

    float invDet = 1.0f / det;
    Matrix4x4 result;
    result.m[0][0] = (m[1][1] * b5 - m[1][2] * b4 + m[1][3] * b3) * invDet;
    result.m[0][1] = (-m[0][1] * b5 + m[0][2] * b4 - m[0][3] * b3) * invDet;
    result.m[0][2] = (m[3][1] * a5 - m[3][2] * a4 + m[3][3] * a3) * invDet;
    result.m[0][3] = (-m[2][1] * a5 + m[2][2] * a4 - m[2][3] * a3) * invDet;
    result.m[1][0] = (-m[1][0] * b5 + m[1][2] * b2 - m[1][3] * b1) * invDet;
    result.m[1][1] = (m[0][0] * b5 - m[0][2] * b2 + m[0][3] * b1) * invDet;
    result.m[1][2] = (-m[3][0] * a5 + m[3][2] * a2 - m[3][3] * a1) * invDet;
    result.m[1][3] = (m[2][0] * a5 - m[2][2] * a2 + m[2][3] * a1) * invDet;
    result.m[2][0] = (m[1][0] * b4 - m[1][1] * b2 + m[1][3] * b0) * invDet;
    result.m[2][1] = (-m[0][0] * b4 + m[0][1] * b2 - m[0][3] * b0) * invDet;
    result.m[2][2] = (m[3][0] * a4 - m[3][1] * a2 + m[3][3] * a0) * invDet;
    result.m[2][3] = (-m[2][0] * a4 + m[2][1] * a2 - m[2][3] * a0) * invDet;
    result.m[3][0] = (-m[1][0] * b3 + m[1][1] * b1 - m[1][2] * b0) * invDet;
    result.m[3][1] = (m[0][0] * b3 - m[0][1] * b1 + m[0][2] * b0) * invDet;
    result.m[3][2] = (-m[3][0] * a3 + m[3][1] * a1 - m[3][2] * a0) * invDet;
    result.m[3][3] = (m[2][0] * a3 - m[2][1] * a1 + m[2][2] * a0) * invDet;

    return result;
}
// 加算
Matrix4x4 operator + (const Matrix4x4& lhs, const Matrix4x4& rhs) {
    Matrix4x4 result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = lhs.m[i][j] + rhs.m[i][j];
        }
    }
    return result;
}
// 減算
Matrix4x4 operator - (const Matrix4x4& lhs, const Matrix4x4& rhs) {
    Matrix4x4 result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = lhs.m[i][j] - rhs.m[i][j];
        }
    }
    return result;
}
// スカラー倍
Matrix4x4 operator * (const Matrix4x4& lhs, float rhs) {
    Matrix4x4 result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = lhs.m[i][j] * rhs;
        }
    }
    return result;
}

Matrix4x4 operator * (float lhs, const Matrix4x4& rhs) {
    return rhs * lhs;
}
// 行列の積
Matrix4x4 operator * (const Matrix4x4& lhs, const Matrix4x4& rhs) {
    Matrix4x4 result = Matrix4x4::zero();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                result.m[i][j] += lhs.m[i][k] * rhs.m[k][j];
            }
        }
    }
    return result;
}
// 演算子オーバーロード (ベクトルとの乗算)
Vector3 operator*(const Vector3& lhs, const Matrix4x4& rhs) {
    return rhs.multiply_point(lhs);
}
// 演算子オーバーロード (加算)
Matrix4x4& operator += (Matrix4x4& lhs, const Matrix4x4& rhs) {
    return lhs = lhs + rhs;
}

// 演算子オーバーロード (減算)
Matrix4x4& operator -= (Matrix4x4& lhs, const Matrix4x4& rhs) {
    return lhs = lhs - rhs;
}

// 演算子オーバーロード (スカラー倍)
Matrix4x4& operator *= (Matrix4x4& lhs, float rhs) {
    return lhs = lhs * rhs;
}

// 演算子オーバーロード (乗算)
Matrix4x4& operator *= (Matrix4x4& lhs, const Matrix4x4& rhs) {
    return lhs = lhs * rhs;
}