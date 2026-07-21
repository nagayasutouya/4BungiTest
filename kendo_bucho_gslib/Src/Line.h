#ifndef LINE_H_
#define LINE_H_

#include"GTLib/GTLib.h"

// 線分クラス
class Line {
public:
	// デフォルトコンストラクタ
	Line() = default;
	// コンストラクタ
	Line(const Vector3& start, const Vector3& end) :
		start{ start }, end{ end }{}

public:
	// 始点
	Vector3 start{ 0.0f, 0.0f, 0.0f };
	// 終点
	Vector3 end{ 0.0f, 0.0f, 0.0f };
};

#endif
