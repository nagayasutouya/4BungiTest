#include "Random.h"
#include <random>
#include <ctime>
#include <algorithm>

namespace {
    // 乱数生成器 (メルセンヌ・ツイスタ)
    std::mt19937 s_engine;
    // 設定された現在のシード値
    unsigned int s_seed = 0;
}

// シード値を設定する
void Random::seed(unsigned int value) {
    s_seed = value;
    s_engine.seed(s_seed);
}

// シード値を取得する
unsigned int Random::seed() {
    return s_seed;
}

// ランダム化（現在時刻で初期化）
void Random::randomize() {
    // 現在時刻をシード値に設定
    unsigned int new_seed = (unsigned int)std::time(nullptr);
    seed(new_seed);
}

// 整数範囲 [min, max] ※maxを含む乱数が返る
int Random::range(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(s_engine);
}

// 小数範囲 [min, max] ※maxは含まない乱数が返る
float Random::range(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(s_engine);
}