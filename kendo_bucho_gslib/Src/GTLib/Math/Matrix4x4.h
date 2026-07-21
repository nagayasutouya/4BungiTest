#ifndef MATRIX4X4_H_
#define MATRIX4X4_H_

struct Vector3;
struct Quaternion;

// 座標変換行列
struct Matrix4x4 {
    float m[4][4]{};

    // コンストラクタ
    Matrix4x4() = default;
    Matrix4x4(
        float m11, float m12, float m13, float m14,
        float m21, float m22, float m23, float m24,
        float m31, float m32, float m33, float m34,
        float m41, float m42, float m43, float m44);

    // 零行列
    static Matrix4x4 zero();
    // 単位行列
    static Matrix4x4 identity();
    // 拡大縮小行列
    static Matrix4x4 scale(const Vector3& vector);
    // 回転行列
    static Matrix4x4 rotate_x(float degree);
    static Matrix4x4 rotate_y(float degree);
    static Matrix4x4 rotate_z(float degree);
    static Matrix4x4 rotate(float degree, const Vector3& axis);
    static Matrix4x4 rotate(const Quaternion& q);
    // 平行移動行列
    static Matrix4x4 translate(const Vector3& vector);
    // TRS行列 (平行移動、回転、拡大を合成)
    static Matrix4x4 TRS(const Vector3& p, const Quaternion& q, const Vector3& s);

    // 視野変換行列 (DirectX 右手座標系)
    static Matrix4x4 look_at(const Vector3& eye, const Vector3& at, const Vector3& up);
    // 透視変換行列 (DirectX 右手座標系)
    static Matrix4x4 perspective(float fov, float aspect, float zn, float zf);

    // 座標変換 (w除算あり)
    Vector3 multiply_point(const Vector3& point) const;
    // 座標変換 (4x3)
    Vector3 multiply_point4x3(const Vector3& point) const;
    // ベクトル変換 (平行移動なし)
    Vector3 multiply_vector(const Vector3& point) const;

    // スケール値、回転、方向の取得
    Vector3 scale() const;
    // 回転クォータニオンの取得
    Quaternion rotation() const;
    // ワールド座標系での前方向
    Vector3 forward() const;
    // ワールド座標系での上方向
    Vector3 up() const;
    // ワールド座標系での左方向
    Vector3 left() const;
    // ワールド座標系での位置
    Vector3 position() const;

    // 逆行列
    Matrix4x4 inverse() const;
    // 転置行列
    Matrix4x4 transpose() const;
};

// 算術演算
Matrix4x4 operator + (const Matrix4x4& lhs, const Matrix4x4& rhs);
Matrix4x4 operator - (const Matrix4x4& lhs, const Matrix4x4& rhs);
Matrix4x4 operator * (const Matrix4x4& lhs, float rhs);
Matrix4x4 operator * (float lhs, const Matrix4x4& rhs);
Matrix4x4 operator * (const Matrix4x4& lhs, const Matrix4x4& rhs);
Vector3 operator * (const Vector3& lhs, const Matrix4x4& rhs);

// 代入演算
Matrix4x4& operator += (Matrix4x4& lhs, const Matrix4x4& rhs);
Matrix4x4& operator -= (Matrix4x4& lhs, const Matrix4x4& rhs);
Matrix4x4& operator *= (Matrix4x4& lhs, float rhs);
Matrix4x4& operator *= (Matrix4x4& lhs, const Matrix4x4& rhs);

#endif

