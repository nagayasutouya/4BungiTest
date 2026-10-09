#ifndef RAY_H_
#define RAY_H_

#include"GTLib/GTLib.h"

// レイクラス
class Ray {
public:
	// デフォルトコンストラクタ
	Ray() = default;
	// コンストラクタ
	Ray(const Vector3& position, const Vector3& direction) :
		position{ position }, direction{ direction }{}

public:
	// 座標
	Vector3 position{ 0.0f, 0.0f, 0.0f };
	// 方向
	Vector3 direction{ 0.0f, 0.0f, 0.0f };
};

#endif
