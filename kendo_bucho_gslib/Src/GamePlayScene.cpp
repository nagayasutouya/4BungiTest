#include "GamePlayScene.h"
#include "Player.h"
#include "Field.h"
#include "Light.h"
#include "EnemyGenerator.h"
#include "CameraTPS.h"
#include "Radar.h"
#include "Assets.h"

// 開始
void GamePlayScene::start() {
    // 剣道部長メッシュの読み込み
    Graphics::load_skinned_mesh(Mesh_Player, "Assets/model/Kendo/Kendo_mesh.txt", "Assets/model/Kendo/Kendo_skeleton.txt", "Assets/model/Kendo/Kendo_animations.txt");
    // 剣道部長メッシュの読み込み
    Graphics::load_instance_skinned_mesh(Mesh_Enemy, "Assets/model/Karate/Karate_mesh.txt", "Assets/model/Karate/Karate_skeleton.txt", "Assets/model/Karate/Karate_animations.txt");

    // スカイボックス用テクスチャの読み込み
    Graphics::load_mesh(Texture_Skybox, "Assets/Skydome/skydome_mesh.txt");
    // 校庭オクトリーの読み込み
    Graphics::load_mesh(Octree_Koutei, "Assets/model/Koutei/koutei_mesh.txt");
    // 校庭衝突判定用オクトリーの読み込み
    //Graphics::load_mesh(Octree_KouteiCollider, "Assets/model/Koutei/koutei_collider.oct");

    // 数値用テクスチャの読み込み
    Graphics::load_texture(Texture_Number, "Assets/texture/num.png");
    // 文字用テクスチャの読み込み
    Graphics::load_texture(Texture_Text, "Assets/texture/text.png");

    // リザルト中の背景の読み込み
    Graphics::load_texture(Texture_BlueBack, "Assets/texture/blue.png");
    // 評価タイトルの読み込み
    Graphics::load_texture(Texture_Result1, "Assets/texture/result1.png");
    // 評価文章 の読み込み
    Graphics::load_texture(Texture_Result2, "Assets/texture/result2.png");
    // 剣道部長画像の読み込み
    Graphics::load_texture(Texture_Kendo, "Assets/texture/mini_kendo.png");
    // 開始ボタンを押忍の読み込み
    Graphics::load_texture(Texture_Start, "Assets/texture/osu.png");
    // レーダーの背景の画像を読み込み
    Graphics::load_texture(Texture_Radar, "Assets/texture/radar.png");
    // レーダーの点の画像を読み込み
    Graphics::load_texture(Texture_RadarPoint, "Assets/texture/pt.png");


    // ゲーム開始時の効果音の読み込み
    Sound::load_se(Se_GameStart, "Assets/sound/Select.wav", 1);
    // 剣道部長攻撃時効果音の読み込み
    Sound::load_se(Se_PlayerAttack, "Assets/sound/Attack1.wav", 1);
    // 剣道部長ダメージ効果音の読み込み
    Sound::load_se(Se_PlayerDamage, "Assets/sound/Damage2.wav", 1);
    // 空手部長ダメージ効果音の読み込み
    Sound::load_se(Se_EnemeyDamage, "Assets/sound/Attack2.wav", 1);
    // タイムアウトの効果音の読み込み
    Sound::load_se(Se_Timeout, "Assets/sound/timeend.wav", 1);

    // ゲームプレイ中用BGMの読み込み
    Sound::load_bgm(Sound_PlayingBGM, "Assets/sound/kendo.wav", true);
    // リザルト用BGMの読み込み
    Sound::load_bgm(Sound_ResultBGM, "Assets/sound/ed.wav", true);

    // BGMの再生
    Sound::play(Sound_PlayingBGM);
    // 開始効果音（いくぜ！の掛け声）
    Sound::play(Se_GameStart);

    // フィールドクラスの追加
    world_.add_field(new Field{ Octree_Koutei, Octree_KouteiCollider, Texture_Skybox });
    // カメラクラスの追加
    world_.add_camera(new CameraTPS{ &world_, Vector3{ 0.0f, 2.0f, -4.0f }, Vector3{ 0.0f, 1.0f, 0.0f } });
    // ライトクラスの追加
    world_.add_light(new Light{ &world_ });
    // プレーヤーを追加
    world_.add_actor(new Player{ &world_, Vector3{ 0.0f, 0.0f, 0.0f } });
    // 敵生成の追加
    world_.add_enemy(new EnemyGenerator{ &world_,Mesh_Enemy,1000,0,true,(unsigned int)Graphics::get_instance_skinned_mesh_bone_count(Mesh_Enemy) });
    // レーダークラスを追加
    world_.add_actor(new Radar{ &world_ });

    // スコアの初期化
    world_.score().initialize(0);
    // タイマの初期化
    world_.timer().initialize(60.0f);

    // リザルトの初期化
    result_.initialize();
    // ランキングファイルの読み込み
    result_.load("Assets/ranking.txt");
    // 状態の初期化
    state_ = State::Playing;
    //　シーン終了フラグの初期化
    is_end_ = false;
}

// 更新
void GamePlayScene::update(float delta_time) {
    // 状態別の更新
    switch (state_) {
    case State::Playing: update_playing(delta_time); break;
    case State::Result:  update_result(delta_time);  break;
    }
}

// 描画
void GamePlayScene::draw() const {
    // ワールドの描画
    world_.draw();
    // リザルト中はリザルトを描画
    if (state_ == State::Result) {
        // リザルトの描画
        result_.draw();
        // リザルト開始１秒後に「開始ボタンを押忍！」を点滅表示する
        if (result_timer_ >= 60.0f && fmod(result_timer_, 40.0f) < 20.0f) {
            static const Vector2 position{ 250, 700 };
            Graphics::draw_sprite2d(Texture_Start, position, NULL, NULL, NULL, NULL, 0.0f);
        }
    }
}

// 終了しているか？
bool GamePlayScene::is_end() const {
    return is_end_;
}

// 次のシーンを返す
std::string GamePlayScene::next() const {
    return "TitleScene";
}

// 終了
void GamePlayScene::end() {
    // BGMの停止
    Sound::stop_all();
    Graphics::wait_for_gpu();
    // ランキングファイルの保存
    result_.save("Assets/ranking.txt");
    // ワールドを消去
    world_.clear();
    // アセットの削除
    Graphics::delete_skinned_mesh(Mesh_Player);
    Graphics::delete_skinned_mesh(Mesh_Enemy);
    Graphics::delete_mesh(Octree_Koutei);
    Graphics::delete_texture(Texture_Skybox);
    Graphics::delete_texture(Texture_Number);
    Graphics::delete_texture(Texture_Text);
    Graphics::delete_texture(Texture_BlueBack);
    Graphics::delete_texture(Texture_Result1);
    Graphics::delete_texture(Texture_Result2);
    Graphics::delete_texture(Texture_Start);
    Graphics::delete_texture(Texture_Radar);
    Graphics::delete_texture(Texture_RadarPoint);

    Sound::delete_sound_effect(Sound_PlayingBGM);
    Sound::delete_sound_effect(Sound_ResultBGM);
    Sound::delete_sound_effect(Se_GameStart);
    Sound::delete_sound_effect(Se_PlayerAttack);
    Sound::delete_sound_effect(Se_PlayerDamage);
    Sound::delete_sound_effect(Se_EnemeyDamage);
    Sound::delete_sound_effect(Se_Timeout);
}

// ゲームプレイ中の更新
void GamePlayScene::update_playing(float delta_time) {
    // ワールドクラスの更新
    world_.update(delta_time);
    // ゲームオーバーになったらリザルト状態に遷移
    if (world_.is_game_over()) {
        // タイムアウトの効果音を再生
        Sound::play(Se_Timeout);
        // BGMを停止
        Sound::stop_all();
        // リザルトシーン用のBGMを再生
        Sound::play(Sound_ResultBGM);

        // 点数を追加
        result_.add_score(world_.score().get());
        // リザルト中タイマを初期化
        result_timer_ = 0.0f;
        // リザルト状態に遷移
        state_ = State::Result;
    }
}

// リザルト中の更新
void GamePlayScene::update_result(float delta_time) {
    // リザルトの更新
    result_.update(delta_time);
    // リザルト中タイマの更新
    result_timer_ += delta_time;
    // 最低1秒間はリザルト表示、スペースキーを押したらシーン終了
    if (result_timer_ >= 60.0f && Input::get_key_down('V')) {
        is_end_ = true;
    }
}
