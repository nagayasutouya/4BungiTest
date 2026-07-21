#ifndef FIELD_H_
#define FIELD_H_

#include"GTLib/GTLib.h"

class Ray;
class Line;
class BoundingSphere;
class Plane;

// フィールドクラス
class Field {
public:
	// コンストラクタ
	Field(unsigned int octree, unsigned int collider, unsigned int skybox);

	// 更新
	void update(float delta_time);
	// 描画
	void draw() const;

	// 線分との衝突判定
	bool collide(const Line& line,
		Vector3* intersect = nullptr, Plane* plane = nullptr) const;
	// レイとの衝突判定
	bool collide(const Ray& ray, float max_distance,
		Vector3* intersect = nullptr, Plane* plane = nullptr) const;
	// 球体との衝突判定
	bool collide(const BoundingSphere& sphere, Vector3* center = nullptr) const;

	// コピー禁止
	Field(const Field& other) = delete;
	Field& operator = (const Field& other) = delete;

private:
	// 描画用オクトリー
	unsigned int	octree_;
	// 衝突判定用オクトリー
	unsigned int	collider_;
	// スカイボックス
	unsigned int	skybox_;
};

#endif
