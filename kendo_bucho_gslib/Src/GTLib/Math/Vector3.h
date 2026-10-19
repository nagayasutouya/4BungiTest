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
    // ベクトルの2乗の長さ
    float sqr_dot_magnitude() const;

    // 内積
    static float dot(const Vector3& lhs, const Vector3& rhs);
    // 外積
    static Vector3 cross(const Vector3& lhs, const Vector3& rhs);
    // ピッチとヨーからベクトル生成
    static Vector3 create_from_pitch_yaw(float pitch, float yaw);
    // 線形補間
    static Vector3 lerp(const Vector3& a, const Vector3& b, float t);
    // 2点間の距離の2乗
    static float sqr_distance(const Vector3& a, const Vector3& b);
    //from to間の角度
    static float angle(const Vector3& from, const Vector3& to);
    //from to間の符号付き角度
    static float signed_angle(const Vector3& from, const Vector3& to, const Vector3& axis = up());
    //aとbの間の距離
    static float distance(const Vector3& a, const Vector3& b);
    // Vector3(0, 0, 0) と同じ意味
    static Vector3 zero();
    // Vector3(1, 1, 1) と同じ意味
    static Vector3 one();
    // Vector3(0, 1, 0) と同じ意味
    static Vector3 up();
    // Vector3(0, -1, 0) と同じ意味
    static Vector3 down();
    // Vector3(1, 0, 0) と同じ意味
    static Vector3 left();
    // Vector3(-1, 0, 0) と同じ意味
    static Vector3 right();
    // Vector3(0, 0, 1) と同じ意味
    static Vector3 forward();
    // Vector3(0, 0, -1) と同じ意味
    static Vector3 back();
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

