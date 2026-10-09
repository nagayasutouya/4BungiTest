#ifndef QUATERNION_H_
#define QUATERNION_H_

struct Vector3;

// クォータニオン
struct Quaternion {
    float x{ 0.0f };
    float y{ 0.0f };
    float z{ 0.0f };
    float w{ 1.0f };

    // コンストラクタ
    Quaternion(float x = 0.0, float y = 0.0, float z = 0.0, float w = 1.0);

    // 逆クォータニオン
    Quaternion inverse() const;
    // 正規化されたクォータニオンを返す
    Quaternion normalized() const;

    // 内積
    static float dot(const Quaternion& lhs, const Quaternion& rhs);

    // 軸と角度からの生成
    static Quaternion angle_axis(float angle, const Vector3& axis);
    // オイラー角からの生成 (Z, X, Yの順)
    static Quaternion euler(const Vector3& euler);
    static Quaternion euler(float x, float y, float z);

    // 2つの回転aとb間の角度
    static float angle(const Quaternion& a, const Quaternion& b);

    // 前方ベクトルと上ベクトルからの生成
    static Quaternion look_rotation(const Vector3& forward, const Vector3& up);
    // fromからtoへの回転クォータニオンを生成
    static Quaternion from_to_rotation(const Vector3& from, const Vector3& to);
    // fromからtoへの回転クォータニオンを生成
    static Quaternion rotate_towards(const Quaternion& from, const Quaternion& to, float max_degrees_delta);
    // 球面線形補間
    static Quaternion slerp(const Quaternion& a, const Quaternion& b, float t);
    
    //オイラー角の取得
    Vector3 euler_angles()const;
};

// 単項マイナス (全要素の反転)
Quaternion operator - (const Quaternion& lhs);

// クォータニオンの演算オーバーロード
Quaternion operator + (const Quaternion& lhs, const Quaternion& rhs);
Quaternion operator * (const Quaternion& lhs, float rhs);
Quaternion operator * (float lhs, const Quaternion& rhs);

// クォータニオン同士の掛け算
Quaternion operator * (const Quaternion& lhs, const Quaternion& rhs);

// ベクトルとの掛け算 (クォータニオンによる回転)
Vector3 operator * (const Quaternion& rotation, const Vector3& point);

#endif

