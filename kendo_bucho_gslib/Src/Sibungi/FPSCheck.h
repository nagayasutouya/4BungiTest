#ifndef FPS_CHECK_H_
#define FPS_CHECK_H_

#include <iostream>
#include <chrono>
#include <thread>

class FPSCheck {
public:
	//コンストラクタ
	FPSCheck();
	//インスタンス化
	static FPSCheck* Instance() {
		static FPSCheck op;
		return &op;
	}
	//計算
	void update();
	//FPSを返す
	double fps()const { return fps_; }
private:
	//クラス名
	using clock = std::chrono::high_resolution_clock;
	//時間取得の型
	using time_point = clock::time_point;
	//fps
	double fps_{0.0f};
	//ゲーム開始時の時間
	time_point prev_time_;
	//フレームのカウント
	int frame_count_{ 0 };
};

#endif