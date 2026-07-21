#ifndef BOUNDING_SPHERE_H_
#define BOUNDING_SPHERE_H_

#include"GTLib/GTLib.h"

// 境界球クラス
class BoundingSphere {
public:
	// コンストラクタ
	BoundingSphere(float radius = 0.0f, const Vector3& center = Vector3{ 0.0f, 0.0f, 0.0f });
	// 平行移動
	BoundingSphere translate(const Vector3& position) const;
	// 座標変換
	BoundingSphere transform(const Matrix4x4& matrix) const;
	// 重なっているか？
	bool intersects(const BoundingSphere& other) const;
	// デバッグ表示
	void draw() const;

public:
	// 半径
	float radius{ 0.0f };
	// 中心座標
	Vector3 center{ 0.0f, 0.0f, 0.0f };
};

#endif
