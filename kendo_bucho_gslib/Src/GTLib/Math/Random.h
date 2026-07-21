#ifndef RANDOM_H_
#define RANDOM_H_


// 乱数ユーティリティ
namespace Random {
    // シード値を設定する
    void seed(unsigned int value);
    // 現在のシード値（最後にセットした値）を取得する
    unsigned int seed();
    // システムのデバイスや時刻を使用してシードをランダム化する
    void randomize();
    // 指定した範囲内の整数をランダムに取得する [min, max]
    int range(int min, int max);
    // 指定した範囲内の浮動小数をランダムに取得する [min, max]
    float range(float min, float max);
}

#endif

