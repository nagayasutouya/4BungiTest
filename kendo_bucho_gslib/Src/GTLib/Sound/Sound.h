#ifndef SOUND_H_
#define SOUND_H_

#include <string>
#include <cstdint>


class SoundEffect;

// サウンド関連の管理クラス
namespace Sound {
    // 初期化・終了
    // XAudio2・Media Foundationの初期化
    void initialize();
    // 終了
    void finalize();

    // ボリュームコントロール
    // 全体の音量設定(0.0f～1.0f)
    void set_master_volume(float volume);
    // BGMの音量設定(0.0f～1.0f)
    void set_bgm_volume(float volume);
    // SEの音量設定(0.0f～1.0f)
    void set_se_volume(float volume);

    // BGMの読み込み (デフォルト: 同時再生1, ループあり)
    SoundEffect* load_bgm(std::uint32_t id, const std::string& filename, std::uint32_t max_voices = 1, bool loop = true, float volume = 1.0f);
    // SEの読み込み (デフォルト: 同時再生4, ループなし)
    SoundEffect* load_se(std::uint32_t id, const std::string& filename, std::uint32_t max_voices = 4, bool loop = false, float volume = 1.0f);

    // サウンドの削除
    void delete_sound_effect(std::uint32_t id);
    // インスタンスの取得
    SoundEffect* get_sound_effect(std::uint32_t id);

    // 再生コントロール
    // サウンドの再生
    void play(std::uint32_t id, bool overlap = true, float volume = 1.0f, float pitch = 1.0f);

    // 指定サウンドの全ボイス停止
    void stop(std::uint32_t id);
    // 全てのサウンド停止
    void stop_all();

    // 指定サウンドの一時停止
    void pause(std::uint32_t id);
    // 全てのサウンド一時停止
    void pause_all();

    // 指定サウンドの再開
    void resume(std::uint32_t id);
    // 全てのサウンド再開
    void resume_all();
}

#endif

