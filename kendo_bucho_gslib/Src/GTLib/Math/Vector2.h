#ifndef VECTOR2_H_
#define VECTOR2_H_


// 2Dベクトル
struct Vector2 {
    // ベクトルのx成分
    float x{ 0.0f };
    // ベクトルのy成分
    float y{ 0.0f };


    // デフォルトコンストラクタ
    Vector2() = default;
    // コンストラクタ
    Vector2(float x, float y);


    // ベクトルの長さ
    float magnitude() const;
    // 正規化したベクトルを返す
    Vector2 normalized() const;


    // 2つのベクトルの内積
    static float dot(const Vector2& lhs, const Vector2& rhs);
    // 最小値と最大値の範囲内にクランプしたベクトルを返す
    static Vector2 clamp(const Vector2& v, const Vector2& min, const Vector2& max);
    // 線形補間
    static Vector2 lerp(const Vector2& a, const Vector2& b, float t);
    Vector2 lerp(const Vector2& a, float t);
};


// 単項演算子オーバーロード
Vector2 operator - (const Vector2& v);                          // 逆ベクトル
// ２項演算子オーバーロード
Vector2 operator + (const Vector2& lhs, const Vector2& rhs);    // 加算
Vector2 operator - (const Vector2& lhs, const Vector2& rhs);    // 減算
Vector2 operator * (const Vector2& lhs, float rhs);             // スカラー倍 V * k
Vector2 operator * (float lhs, const Vector2& rhs);             // スカラー倍 k * V
Vector2 operator / (const Vector2& lhs, float rhs);             // スカラーで除算
// 複合代入演算子
Vector2& operator += (Vector2& lhs, const Vector2& rhs);
Vector2& operator -= (Vector2& lhs, const Vector2& rhs);
Vector2& operator *= (Vector2& lhs, float rhs);
Vector2& operator /= (Vector2& lhs, float rhs);

#endif