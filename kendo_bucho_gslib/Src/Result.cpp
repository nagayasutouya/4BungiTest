#include "Result.h"
#include "NumberTexture.h"
#include "Assets.h"
#include"GTLib/GTLib.h"
#include <fstream> // ファイル操作に必要

// ランキング数
const int RankingCount{ 5 };
// プレーヤーの移動時間
const float MovingTime{ 30.0f };

// コンストラクタ
Result::Result() {
    // 初期化する
    initialize();
}

// 初期化
void Result::initialize() {
    // プレーヤー移動用タイマの初期化
    moving_timer_ = 0.0f;
    // 最終スコアの初期化
    final_score_ = 0;
    // 最終ランクの初期化
    final_rank_ = 0;
    // ランキングの初期化
    ranking_scores_.resize(RankingCount, 0);
}

// スコアの追加
void Result::add_score(int value) {
    // スコアの設定
    final_score_ = value;
    // 順位を求める
    final_rank_ = 0;
    for (int score : ranking_scores_) {
        // 追加するスコア以上の既存のスコアの数を数える
        if (final_score_ <= score) {
            ++final_rank_;
        }
    }
    // ランクインしたか？
    if (final_rank_ < RankingCount) {
        // 新しいスコアの挿入
        ranking_scores_.insert(ranking_scores_.begin() + final_rank_, final_score_);
        // 最下位のスコアを削除
        ranking_scores_.pop_back();
    }
}

// 更新
void Result::update(float delta_time) {
    // 移動時間の更新
    moving_timer_ = Mathf::Min(moving_timer_ + delta_time, MovingTime);
}

// 描画
void Result::draw() const {
    // 背景を描画
    draw_background();
    // 剣道部長の表示
    draw_player();
    // 評価メッセージの表示
    draw_grade_message();
    // ランキングの表示
    draw_ranking();
}

// ランキングファイルの保存
void Result::save(const std::string& file_name) const {
    // ファイルのオープン
    std::ofstream file{ file_name };
    // ファイルがオープンできなかったら何もしない
    if (!file) return;
    // スコアの保存
    for (int score : ranking_scores_) {
        file << score << std::endl;
    }
}

// ランキングファイルの読み込み
void Result::load(const std::string& file_name) {
    // ファイルのオープン
    std::ifstream file{ file_name };
    // ファイルがオープンできなかったら何もしない
    if (!file) return;
    // スコアの読み込み
    for (int& score : ranking_scores_) {
        file >> score;
    }
}

// 背景の描画
void Result::draw_background() const {
    // 半透明の青い背景を表示 
    static const Color bg_color{ 1.0f, 1.0f, 1.0f, 0.5f };
    static const Vector2 bg_position{ 20.0f, 100.0f };
    Graphics::draw_sprite2d(Texture_BlueBack, bg_position, NULL, NULL, &bg_color, NULL, 0.0f);
}

// プレーヤーの描画
void Result::draw_player() const {
    // 剣道部長の画像の座標を線形補間で動かす
    static const Vector2 kendo_position_start{ -1024.0f, 100.0f };    // 開始位置
    static const Vector2 kendo_position_end{ 370.0f, 100.0f };        // 終了位置
    Vector2 kendo_position = Vector2::lerp(kendo_position_start,
        kendo_position_end, moving_timer_ / MovingTime);
    // プレーヤー画像の描画
    Graphics::draw_sprite2d(Texture_Kendo, kendo_position, NULL, NULL, NULL, NULL, 0.0f);
}

// ランキングの描画
void Result::draw_ranking() const {
    // 「順位」の文字を表示
    static const Rect    jyuni_rect{ 0, 192, 128, 256 };        // 「順位」の切り出し位置
    static const Vector2 jyuni_position{ 40.0f, 360.0f };       // 「順位」の表示座標
    static const Color   jyuni_color{ 1.0f, 0.0f, 0.0f, 1.0f }; // 「順位」の色（赤）
    Graphics::draw_sprite2d(Texture_Text, jyuni_position, &jyuni_rect, NULL, &jyuni_color, NULL, 0.0f);
    // 上位５位の順位（主席など）と得点を描画
    for (int i = 0; i < (int)ranking_scores_.size(); ++i) {
        Color rank_in_color{ 1.0f, 1.0f, 1.0f, 1.0f }; // 白
        // ランクインしたら、その順位の文字を赤くする
        if (i == final_rank_) {
            rank_in_color = Color{ 1.0f, 0.0f, 0.0f, 1.0f }; // 赤
        }
        // 「主席」「次席」「３位」「４位」「５位」のランクを描画
        const Rect rank_rect{ 128.0f, 60.0f * i, 256.0f, 60.0f * i + 60.0f };
        const Vector2 rank_position{ 180.0f, 360.0f + 60.0f * i };
        Graphics::draw_sprite2d(Texture_Text, rank_position, &rank_rect, NULL, &rank_in_color, NULL, 0.0f);
        // ランキングデータ内の得点を描画
        static const NumberTexture number{ Texture_Number, 32, 64 };
        number.draw(Vector2{ 320.0f, 360.0f + 60.0f * i }, ranking_scores_[i], 3, '0', rank_in_color);
    }
}

// 評価メッセージの描画
void Result::draw_grade_message() const {
    // スコアから評価を計算する
    int grades = calculate_grades();
    // 評価のタイトルを表示
    Rect grades1_rect{ 0.0f, 64.0f * grades, 1024, 64.0f * grades + 64.0f };
    static const Vector2 grades1_position{ 40.0f, 120.0f };
    static const Color grades1_color{ 1.0f, 0.0f, 0.0f, 1.0f };
    Graphics::draw_sprite2d(Texture_Result1, grades1_position, &grades1_rect, NULL, &grades1_color, NULL, 0.0f);
    // 評価の文面を表示
    Rect grades2_rect{ 0.0f, 200.0f * grades, 1024, 200.0f * grades + 180.0f };
    static const Vector2 grades2_position{ 40.0f, 190.0f };
    Graphics::draw_sprite2d(Texture_Result2, grades2_position, &grades2_rect, NULL, NULL, NULL, 0.0f);
}

// スコアから評価を計算する
int Result::calculate_grades() const {
    if (final_score_ >= 60) return 0;   // 熱血硬派 剣道部長
    if (final_score_ >= 50) return 1;   // 素晴らしい剣道部長
    if (final_score_ >= 40) return 2;   // 普通の剣道部長
    if (final_score_ >= 30) return 3;   // 勘違の剣道部長
    return 4;                           // 軟弱な剣道部長
}

