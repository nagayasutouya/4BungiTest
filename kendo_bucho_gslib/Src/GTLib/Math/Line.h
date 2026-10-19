#ifndef LINE_H_
#define LINE_H_

#include"GTLib/GTLib.h"
// 線分構造体
class Line {
public:
	// 線分の始点座標
	Vector3 start{};
	// 線分の終点座標
	Vector3 end{};
	// デフォルトコンストラクタ
	Line() = default;
	// コンストラクタ
	Line(const Vector3& s, const Vector3& e);
};

// 線分判定結果構造体
struct LineHit {
	// 衝突したか？
	bool hit{ false };
	// 衝突点
	Vector3 point{};
	// 衝突面の法線
	Vector3 normal{};
	// 始点からの距離
	float distance{ 0.0f };
};

#endif
