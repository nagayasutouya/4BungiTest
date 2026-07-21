#include "Transform.h"
#include <cmath>

// ワールド空間での Transform の青軸を取得
Vector3 Transform::forward() const {
    return rotation_ * Vector3{ 0.0f, 0.0f, 1.0f };
}

// ワールド空間での Transform の青軸を設定
void Transform::forward(const Vector3& value) {
    rotation_ = Quaternion::look_rotation(value, up());
}
// ワールド空間での Transform の赤軸を取得
Vector3 Transform::right() const {
    return rotation_ * Vector3{ -1.0f, 0.0f, 0.0f };
}
// ワールド空間での Transform の赤軸を設定
void Transform::right(const Vector3& value) {
    rotation_ = Quaternion::from_to_rotation(Vector3{ -1.0f, 0.0f, 0.0f }, value);
}
// ワールド空間での Transform の緑軸を取得
Vector3 Transform::up() const {
    return rotation_ * Vector3{ 0.0f, 1.0f, 0.0f };
}
// ワールド空間での Transform の緑軸を設定
void Transform::up(const Vector3& value) {
    rotation_ = Quaternion::from_to_rotation(Vector3{ 0.0f, 1.0f, 0.0f }, value);
}
// オブジェクトのグローバルスケールを取得
Vector3 Transform::lossy_scale() const {
    return scale_;
}
// ワールド空間での Transform の位置を設定
void Transform::scale(const Vector3& value) {
    scale_ = value;
}
// ワールド空間での Transform の位置を取得
Vector3 Transform::position() const {
    return position_;
}
// ワールド空間での Transform の位置を設定
void Transform::position(const Vector3& value) {
    position_ = value;
}
// クォータニオンとしての回転を取得
Quaternion Transform::rotation() const {
    return rotation_;
}
// クォータニオンとしての回転を設定
void Transform::rotation(const Quaternion& value) {
    rotation_ = value;
}
// オイラー角としての角度
Vector3 Transform::euler_angles() const {
    return rotation_.euler_angles();
}
// オイラー角としての角度を設定
void Transform::euler_angles(const Vector3& value) {
    rotation_ = Quaternion::euler(value);
}
// 対象の座標を設定し、その方向へと向かせます
void Transform::look_at(const Vector3& target, const Vector3& world_up) {
    rotation_ = Quaternion::look_rotation(target - position_, world_up);
}
// Z 軸でeulers.z度回転、X軸でeuler.x度回転、Y軸でeulers.y度回転します(順番は説明した順)
void Transform::rotate(const float& x, const float& y, const float& z, Space relative_to) {
    rotate(Vector3{ x,y,x }, relative_to);
}
// Z 軸でeulers.z度回転、X軸でeuler.x度回転、Y軸でeulers.y度回転します(順番は説明した順)
void Transform::rotate(const Vector3& eulers, Space relative_to) {
    if (relative_to == Space::Self) {
        rotation_ = rotation_ * Quaternion::euler(eulers);
    }
    else {
        rotation_ = Quaternion::euler(eulers) * rotation_;
    }
}
// axis 軸の周りに angle 度、回転します
void Transform::rotate(const Vector3& axis, float angle, Space relative_to) {
    if (relative_to == Space::Self) {
        rotation_ = rotation_ * Quaternion::angle_axis(angle, axis);
    }
    else {
        rotation_ = Quaternion::angle_axis(angle, axis) * rotation_;
    }
}
// ワールド座標の point を中心とした軸(axis)で angle 度回転させます
void Transform::rotate_around(const Vector3& point, const Vector3& axis, float angle) {
    Quaternion q = Quaternion::angle_axis(angle, axis);
    position_ = point + q * (position_ - point);
    rotation_ = q * rotation_;
}
// translation の方向と距離に移動します
void Transform::translate(const Vector3& translation, Space relative_to) {
    if (relative_to == Space::Self) {
        position_ += rotation_ * translation;
    }
    else {
        position_ += translation;
    }
}
void Transform::translate(const float& x, const float& y, const float& z, Space relative_to) {
    translate(Vector3{ x,y,z }, relative_to);
}
// ローカル座標からワールド座標へ変換した行列
Matrix4x4 Transform::local_to_world_matrix() const {
    return Matrix4x4::TRS(position_, rotation_, scale_);
}
// ワールド座標からローカル座標へ変換した行列
Matrix4x4 Transform::world_to_local_matrix() const {
    return local_to_world_matrix().inverse();
}
// ローカル空間からワールド空間へ position を変換します（スケール・回転・平行移動）
Vector3 Transform::transform_point(const Vector3& position) const {
    return local_to_world_matrix().multiply_point4x3(position);
}
// ローカル空間からワールド空間へ vector を変換します（スケール・回転）
Vector3 Transform::transform_vector(const Vector3& vector) const {
    return local_to_world_matrix().multiply_vector(vector);
}
// ローカル空間からワールド空間へ direction を変換します（回転）
Vector3 Transform::transform_direction(const Vector3& direction) const {
    return rotation_ * direction;
}
// ワールド空間からローカル空間へ position を変換します
Vector3 Transform::inverse_transform_point(const Vector3& position) const {
    return world_to_local_matrix().multiply_point4x3(position);
}
// ワールド空間からローカル空間へ vector を変換します
Vector3 Transform::inverse_transform_vector(const Vector3& vector) const {
    return world_to_local_matrix().multiply_vector(vector);
}
// ワールド空間からローカル空間へ direction を変換します
Vector3 Transform::inverse_transform_direction(const Vector3& direction) const {
    return rotation_.inverse() * direction;
}