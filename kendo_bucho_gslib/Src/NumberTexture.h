#ifndef NUMBER_TEXTURE_H_
#define NUMBER_TEXTURE_H_

#include"GTLib/GTLib.h"
#include <string>

// ナンバーテクスチャクラス
class NumberTexture {
public:
    // コンストラクタ
    NumberTexture(unsigned int texture, int width, int height);
    // 描画
    void draw(const Vector2& position, int num, int digit, char fill = '0',
        const Color& color = Color{ 1.0f, 1.0f, 1.0f, 1.0f }) const;
    // 描画
    void draw(const Vector2& position, int num,
        const Color& color = Color{ 1.0f, 1.0f, 1.0f, 1.0f }) const;
    // 描画
    void draw(const Vector2& position, const std::string& num,
        const Color& color = Color{ 1.0f, 1.0f, 1.0f, 1.0f }) const;


private:
    // フォント用のテクスチャ
    unsigned int   texture_;
    // 文字の幅
    int     width_;
    // 文字の高さ
    int     height_;
};

#endif