#ifndef MATHF_H_
#define MATHF_H_

namespace Mathf {
    // 円周率
    constexpr float PI = 3.1415926535f;

    // 角度からラジアンに変換
    float deg_to_rad(float deg);
    // ラジアンから角度に変換
    float rad_to_deg(float rad);
    //デカい方
    float Max(float a, float b);
    int Max(int a, int b);
    //小さい方
    float Min(float a, float b);
    int Min(int a, int b);
    //値を収める
    float CLAMP(float current, float min, float max);
    int CLAMP(int current, int min, int max);
    //絶対値
    int ABS(int a);
    //乗
    int nth_power(int target, int n);
    float nth_power(float target, int n);
}

#endif