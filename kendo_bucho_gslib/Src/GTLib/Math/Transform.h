#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix4x4.h"

// 物体の位置・回転・スケールを管理するクラス
class Transform {
public:
    enum class Space {
        Self,  // 自身の座標系
        World  // ワールド座標系
    };

public:
    // コンストラクタ
    Transform() = default;
    // デストラクタ
    ~Transform() = default;
    // ワールド座標系での前進方向
    Vector3 forward() const;
    // ワールド座標系での前進方向を設定
    void forward(const Vector3& value);
    // ワールド座標系での右方向
    Vector3 right() const;
    // ワールド座標系での右方向を設定
    void right(const Vector3& value);
    // ワールド座標系での上方向
    Vector3 up() const;
    // ワールド座標系での上方向を設定
    void up(const Vector3& value);

    // オブジェクトのグローバルスケールを取得
    Vector3 lossy_scale() const;
    // スケール値を設定
    void scale(const Vector3& value);
    // ワールド座標系での位置
    Vector3 position() const;
    // ワールド座標系での位置を設定
    void position(const Vector3& value);
    // ワールド座標系での回転
    Quaternion rotation() const;
    // ワールド座標系での回転を設定
    void rotation(const Quaternion& value);
    // オイラー角としての角度
    Vector3 euler_angles() const;
    // オイラー角としての角度を設定
    void euler_angles(const Vector3& value);

    // 対象の座標を設定し、その方向へと向かせます
    void look_at(const Vector3& target, const Vector3& world_up = Vector3{ 0.0f, 1.0f, 0.0f });
    // Z 軸でeulers.z度回転、X軸でeuler.x度回転、Y軸でeulers.y度回転します(順番は説明した順)
    void rotate(const float& x, const float& y, const float& z, Space relative_to = Space::Self);
    // Z 軸でeulers.z度回転、X軸でeuler.x度回転、Y軸でeulers.y度回転します(順番は説明した順)
    void rotate(const Vector3& eulers, Space relative_to = Space::Self);
    // axis軸の周りにangle度回転します
    void rotate(const Vector3& axis, float angle, Space relative_to = Space::Self);
    // ワールド座標のpointを中心としたaxis軸でangle度回転させます
    void rotate_around(const Vector3& point, const Vector3& axis, float angle);

    // translationの方向と距離に移動します
    void translate(const Vector3& translation, Space relative_to = Space::Self);
    void translate(const float& x, const float& y, const float& z, Space relative_to = Space::Self);

    // ローカル座標からワールド座標へ変換した行列
    Matrix4x4 local_to_world_matrix() const;
    // ワールド座標からローカル座標へ変換した行列
    Matrix4x4 world_to_local_matrix() const;

    // ローカル空間からワールド空間へpositionを変換します(スケール・回転・平行移動)
    Vector3 transform_point(const Vector3& position) const;
    // ローカル空間からワールド空間へvectorを変換します(スケール・回転)
    Vector3 transform_vector(const Vector3& vector) const;
    // ローカル空間からワールド空間へdirectionを変換します(回転)
    Vector3 transform_direction(const Vector3& direction) const;

    // ワールド空間からローカル空間へpositionを変換します
    Vector3 inverse_transform_point(const Vector3& position) const;
    // ワールド空間からローカル空間へvectorを変換します
    Vector3 inverse_transform_vector(const Vector3& vector) const;
    // ワールド空間からローカル空間へdirectionを変換します
    Vector3 inverse_transform_direction(const Vector3& direction) const;

private:
    // ポジション (ワールド座標系)
    Vector3 position_{ 0.0f, 0.0f, 0.0f };
    // 回転 (ワールド座標系)
    Quaternion rotation_{ 0.0f, 0.0f, 0.0f, 1.0f };
    // スケール (ワールド座標系)
    Vector3 scale_{ 1.0f, 1.0f, 1.0f };
};

#endif

