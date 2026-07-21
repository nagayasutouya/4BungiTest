#include "Sound.h"
#include "SoundEffect.h"
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <xaudio2.h>
#include <mfapi.h>

// 依存ライブラリ
#pragma comment(lib, "xaudio2.lib")
#pragma comment(lib, "mfplat.lib")

namespace {
    // XAudio2インスタンス
    IXAudio2* s_xaudio2_ = nullptr;
    // マスターボイス(出力全体を制御)
    IXAudio2MasteringVoice* s_master_voice_ = nullptr;

    // サブミックスボイス(カテゴリごとの音量等用)
    IXAudio2SubmixVoice* s_bgm_submix_voice_ = nullptr;
    IXAudio2SubmixVoice* s_se_submix_voice_ = nullptr;

    // リソース管理用マップ (ID -> unique_ptr<SoundEffect>)
    std::unordered_map<std::uint32_t, std::unique_ptr<SoundEffect>> s_sound_effects_;

    // COM初期化されたか
    bool s_co_initialized_ = false;
}

// 初期化
void Sound::initialize() {
    // COMの初期化(Media FoundationやXAudio2で必要)
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (SUCCEEDED(hr)) {
        s_co_initialized_ = true;
    }

    // Media Foundationの初期化
    hr = MFStartup(MF_VERSION);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to initialize Media Foundation");
    }

    // XAudio2の作成
    hr = XAudio2Create(&s_xaudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to initialize XAudio2");
    }

    // マスターボイスの作成
    hr = s_xaudio2_->CreateMasteringVoice(&s_master_voice_);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create XAudio2 Mastering Voice");
    }

    // マスターボイスの設定取得してサブミックスボイスのフォーマット合わせる
    XAUDIO2_VOICE_DETAILS master_details;
    s_master_voice_->GetVoiceDetails(&master_details);

    // BGM用のサブミックスボイス作成
    hr = s_xaudio2_->CreateSubmixVoice(
        &s_bgm_submix_voice_,
        master_details.InputChannels,
        master_details.InputSampleRate,
        0, 0, nullptr, nullptr
    );
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create BGM Submix Voice");
    }

    // SE用のサブミックスボイス作成
    hr = s_xaudio2_->CreateSubmixVoice(
        &s_se_submix_voice_,
        master_details.InputChannels,
        master_details.InputSampleRate,
        0, 0, nullptr, nullptr
    );
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create SE Submix Voice");
    }
}

// 終了処理
void Sound::finalize() {
    // 登録されている音を破棄(unique_ptrなのでclear()でOK)
    s_sound_effects_.clear();

    // BGMサブミックスボイスの破棄
    if (s_bgm_submix_voice_) {
        s_bgm_submix_voice_->DestroyVoice();
        s_bgm_submix_voice_ = nullptr;
    }
    // SEサブミックスボイスの破棄
    if (s_se_submix_voice_) {
        s_se_submix_voice_->DestroyVoice();
        s_se_submix_voice_ = nullptr;
    }
    // マスターボイスの破棄
    if (s_master_voice_) {
        s_master_voice_->DestroyVoice();
        s_master_voice_ = nullptr;
    }
    // XAudio2オブジェクトの解放
    if (s_xaudio2_) {
        s_xaudio2_->Release();
        s_xaudio2_ = nullptr;
    }
    // Media Foundationの終了
    MFShutdown();

    // COMの終了
    if (s_co_initialized_) {
        CoUninitialize();
        s_co_initialized_ = false;
    }
}

// 全体の音量設定
void Sound::set_master_volume(float volume) {
    s_master_voice_->SetVolume(volume);
}

// BGMの音量設定
void Sound::set_bgm_volume(float volume) {
    s_bgm_submix_voice_->SetVolume(volume);
}

// SEの音量設定
void Sound::set_se_volume(float volume) {
    s_se_submix_voice_->SetVolume(volume);
}

// サウンドファイルの読み込み
SoundEffect* load_sound(std::uint32_t id, const std::string& filename,
    std::uint32_t max_voices, bool loop, float volume, IXAudio2SubmixVoice* submix) {
    // 既に同IDが存在する場合は上書きするため削除
    s_sound_effects_.erase(id);

    SoundEffect::Desc desc{};
    desc.max_voices = max_voices;
    desc.loop = loop;
    desc.volume = volume;
    desc.submix_voice = submix;

    auto se = std::make_unique<SoundEffect>(s_xaudio2_, filename, desc);
    SoundEffect* se_ptr = se.get();
    s_sound_effects_[id] = std::move(se);
    return se_ptr;
}

// BGMの読み込み
SoundEffect* Sound::load_bgm(std::uint32_t id, const std::string& filename,
    std::uint32_t max_voices, bool loop, float volume) {
    return load_sound(id, filename, max_voices, loop, volume, s_bgm_submix_voice_);
}

// SEの読み込み
SoundEffect* Sound::load_se(std::uint32_t id, const std::string& filename,
    std::uint32_t max_voices, bool loop, float volume) {
    return load_sound(id, filename, max_voices, loop, volume, s_se_submix_voice_);
}

// サウンドエフェクトの削除
void Sound::delete_sound_effect(std::uint32_t id) {
    s_sound_effects_.erase(id);
}

// サウンドエフェクトの取得
SoundEffect* Sound::get_sound_effect(std::uint32_t id) {
    auto it = s_sound_effects_.find(id);
    if (it != s_sound_effects_.end()) {
        return it->second.get();
    }
    return nullptr;
}

// 再生
void Sound::play(std::uint32_t id, bool overlap, float volume, float pitch) {
    SoundEffect* se = get_sound_effect(id);
    if (se) {
        se->play(overlap, volume, pitch);
    }
}

void Sound::stop(std::uint32_t id) {
    SoundEffect* se = get_sound_effect(id);
    if (se) {
        se->stop();
    }
}

// 全てのサウンドエフェクトを停止
void Sound::stop_all() {
    for (auto& pair : s_sound_effects_) {
        pair.second->stop();
    }
}

// ポーズ
void Sound::pause(std::uint32_t id) {
    SoundEffect* se = get_sound_effect(id);
    if (se) {
        se->pause();
    }
}

// 全てのサウンドエフェクトをポーズ
void Sound::pause_all() {
    for (auto& pair : s_sound_effects_) {
        pair.second->pause();
    }
}

// 再開
void Sound::resume(std::uint32_t id) {
    SoundEffect* se = get_sound_effect(id);
    if (se) {
        se->resume();
    }
}

// 全てのサウンドエフェクトを再開
void Sound::resume_all() {
    for (auto& pair : s_sound_effects_) {
        pair.second->resume();
    }
}