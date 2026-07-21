#ifndef COLOR_H_
#define COLOR_H_

// RGBAカラー
struct Color {
    // 赤成分 (0.0f ～ 1.0f)
    float r{ 0.0f };
    // 緑成分 (0.0f ～ 1.0f)
    float g{ 0.0f };
    // 青成分 (0.0f ～ 1.0f)
    float b{ 0.0f };
    // アルファ成分 (0.0f ～ 1.0f)
    float a{ 1.0f };
};

#endif