#include "BoundingSphere.h"
#include "GTLib/Physics/Collision.h"

// コンストラクタ
BoundingSphere::BoundingSphere(float radius, const Vector3& center) :
	radius{ radius }, center{ center } {
}

// 平行移動
BoundingSphere BoundingSphere::translate(const Vector3& position) const {
	return BoundingSphere{ radius, center + position };
}

// 座標変換
BoundingSphere BoundingSphere::transform(const Matrix4x4& matrix) const {
	return BoundingSphere{ radius * matrix.scale().y, matrix.position()+center};
}

// 交差しているか？
bool BoundingSphere::intersects(const BoundingSphere& other) const {
	return CollisionPhysics::CollisionSphereAndSphere(center, radius, other.center, other.radius) == true;
}

// デバッグ表示
void BoundingSphere::draw() const {}

