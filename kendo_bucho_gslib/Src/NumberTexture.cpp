#include "NumberTexture.h"
#include <sstream>
#include <iomanip>

// コンストラクタ
NumberTexture::NumberTexture(unsigned int texture, int width, int height) :
    texture_{ texture }, width_{ width }, height_{ height } {
}

// 描画
void NumberTexture::draw(const Vector2& position, int num, int digit, char fill, const Color& color) const {
    std::stringstream ss;
    ss << std::setw(digit) << std::setfill(fill) << num;
    draw(position, ss.str(), color);
}

// 描画
void NumberTexture::draw(const Vector2& position, int num, const Color& color) const {
    draw(position, std::to_string(num), color);
}

// 描画
void NumberTexture::draw(const Vector2& position, const std::string& num, const Color& color) const {
    for (int i = 0; i < (int)num.size(); ++i) {
        if (num[i] == ' ') continue;
        const int n = (num[i] != '.') ? num[i] - '0' : 10; // 小数点は9の次にあることを前提
        Rect rect;
        rect.left = (float)(n * width_); 
        rect.top = 0.0f;
        rect.right = (float)((n * width_) + width_);
        rect.bottom=(float)height_;
        const Vector2 pos{ position.x + i * width_, position.y };
        Graphics::draw_sprite2d(texture_, pos, &rect, NULL, &color, NULL, 0);
    }
}