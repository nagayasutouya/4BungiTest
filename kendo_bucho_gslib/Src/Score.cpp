#include "Score.h"
#include "NumberTexture.h"
#include "Assets.h"
#include"GTLib/GTLib.h"

// コンストラクタ
Score::Score(int score) :
    score_{ score } {
}

// スコアの初期化
void Score::initialize(int score) {
    score_ = score;
}

// スコアの加算
void Score::add(int score) {
    score_ = Mathf::Min(score_ + score, 999);
}

// スコアの描画
void Score::draw() const {
    // 「得点」の表示
    static const Rect rect{ 0, 64, 128, 128 };
    static const Vector2 position{ 340, 20 };
    Graphics::draw_sprite2d(Texture_Text, position, &rect, NULL, NULL, NULL, 0.0f);
    // 数値の表示
    static const NumberTexture number{ Texture_Number, 32, 64 };
    number.draw(Vector2{ 480, 20 }, score_, 3);
}

// スコアの取得
int Score::get() const {
    return score_;
}

// スコアのクリア
void Score::clear() {
    score_ = 0;
}

