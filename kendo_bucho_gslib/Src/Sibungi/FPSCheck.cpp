#include "FPSCheck.h"
//コンストラクタ
FPSCheck::FPSCheck(){
	prev_time_ = clock::now();
}
//計算
void FPSCheck::update() {
    frame_count_++;
    auto current_time = clock::now();

    // 前回FPSを更新してからの経過時間（秒）を計算
    std::chrono::duration<double> elapsed = current_time - prev_time_;

    // 1秒ごとにFPSを更新・表示
    if (elapsed.count() >= 1.0) {
        fps_ = static_cast<double>(frame_count_) / elapsed.count();

        // カウンタとタイマーのリセット
        frame_count_ = 0;
        prev_time_ = current_time;
    }
}