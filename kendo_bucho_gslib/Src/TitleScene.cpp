#include "TitleScene.h"
#include "Assets.h"
#include"GTLib/GTLib.h"

// 移動タイマ
const float MovingTimerEnd = 30.0f;

// 開始
void TitleScene::start() {
    // タイトル 
    Graphics::load_texture(Texture_Title, "Assets/texture/Title.png");
    // 剣道部長
    Graphics::load_texture(Texture_Kendo, "Assets/texture/mini_kendo.png");
    // 空手部長
    Graphics::load_texture(Texture_Karate, "Assets/texture/mini_karate.png");
    // 開始ボタンを押忍
    Graphics::load_texture(Texture_Start, "Assets/texture/osu.png");

    // タイトルシーン用BGMの読み込み
    Sound::load_bgm(Sound_TitleBGM, "Assets/sound/title.wav", true);
    // BGMの再生
    Sound::play(Sound_TitleBGM);
    // 終了フラグを初期化
    is_end_ = false;
    // 移動タイマを初期化
    moving_timer_ = 0.0f;
    // 点滅タイマを初期化
    blink_timer_ = 0.0f;
}

// 更新
void TitleScene::update(float delta_time) {
    // 移動タイマの更新
    moving_timer_ = Mathf::Min(moving_timer_ + delta_time, MovingTimerEnd);
    // 点滅タイマの更新
    blink_timer_ = fmod(blink_timer_ + delta_time, 40.0f);
    // スペースキーが押されたらゲーム開始    
    if (Input::get_key_down('V')) {
        is_end_ = true;
    }
}

// 描画
void TitleScene::draw() const {
    // タイトル画像の表示
    Graphics::draw_sprite2d(Texture_Title, Vector2{0.0f,0.0f}, NULL, NULL, NULL, NULL, 0.0f);
    
    // 空手部長の表示
    static const Vector2 karate_position_start{ 1024.0f, 130.0f };
    static const Vector2 karate_position_end{ -500.0f, 130.0f };
    Vector2 karate_position = karate_position_start.lerp(karate_position_start,karate_position_end, moving_timer_ / MovingTimerEnd);
    Graphics::draw_sprite2d(Texture_Karate, karate_position, NULL, NULL, NULL, NULL, 0.0f);
    
    // 剣道部長の表示
    static const Vector2 kendo_position_start{ -1024.0f, 130.0f };
    static const Vector2 kendo_position_end{ 0.0f, 130.0f };
    Vector2 kendo_position = kendo_position_start.lerp(kendo_position_start,kendo_position_end, moving_timer_ / MovingTimerEnd);
    Graphics::draw_sprite2d(Texture_Kendo, kendo_position, NULL, NULL, NULL, NULL, 0.0f);

    // 開始メッセージを点滅表示
    if (blink_timer_ < 20.0f) {
        static const Vector2 position{ 250, 700 };
        Graphics::draw_sprite2d(Texture_Start, position, NULL, NULL, NULL, NULL, 0.0f);
    }
}

// 終了しているか？
bool TitleScene::is_end() const {
    return is_end_;
}

// 次のシーン名を返す
std::string TitleScene::next() const {
    return "GamePlayScene";
}

// 終了
void TitleScene::end() {
    // BGMの停止
    Sound::stop_all();
    Graphics::wait_for_gpu();
    // アセットの削除
    Graphics::delete_texture(Texture_Title);
    Graphics::delete_texture(Texture_Kendo);
    Graphics::delete_texture(Texture_Karate);
    Graphics::delete_texture(Texture_Start);
    Sound::delete_sound_effect(Sound_TitleBGM);
}
