#ifndef VECTOR3_H_
#define VECTOR3_H_

// 3Dベクトル
struct Vector3 {
    // ベクトルのx成分
    float x{ 0.0f };
    // ベクトルのy成分
    float y{ 0.0f };
    // ベクトルのz成分
    float z{ 0.0f };

    // コンストラクタ
    Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f);

    // 長さ
    float magnitude() const;
    // 長さの2乗
    float sqr_magnitude() const;
    // 正規化されたベクトルを返す
    Vector3 normalized() const;

    // 内積
    static float dot(const Vector3& lhs, const Vector3& rhs);
    // 外積
    static Vector3 cross(const Vector3& lhs, const Vector3& rhs);
    // ピッチとヨーからベクトル生成
    static Vector3 create_from_pitch_yaw(float pitch, float yaw);
    // 線形補間
    static Vector3 lerp(const Vector3& a, const Vector3& b, float t);
};

// 演算子オーバーロード
Vector3 operator - (const Vector3& v);
Vector3 operator + (const Vector3& lhs, const Vector3& rhs);
Vector3 operator - (const Vector3& lhs, const Vector3& rhs);
Vector3 operator * (const Vector3& lhs, float rhs);
Vector3 operator * (float lhs, const Vector3& rhs);
Vector3 operator / (const Vector3& lhs, float rhs);

Vector3& operator += (Vector3& lhs, const Vector3& rhs);
Vector3& operator -= (Vector3& lhs, const Vector3& rhs);
Vector3& operator *= (Vector3& lhs, float rhs);
Vector3& operator /= (Vector3& lhs, float rhs);

#endif

