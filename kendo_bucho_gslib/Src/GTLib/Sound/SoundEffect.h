#ifndef SOUND_EFFECT_H_
#define SOUND_EFFECT_H_

#include<xaudio2.h>
#include<vector>
#include<string>

class SoundEffect {
public:
	struct Desc {
		UINT max_voices{ 4 };
		bool loop{ false };
		float volume{ 1.0f };
		IXAudio2SubmixVoice* submix_voice{ nullptr };
	};
public:
	SoundEffect(IXAudio2* xaudio2,const std::string& file_name,const SoundEffect::Desc& desc);
	~SoundEffect(); 
	void clear();
	//overlap:true=同時再生,false=同時再生禁止
	//戻り値:再生したやつのボイスインデックス(-1=失敗)
	int play(bool overlap=true,float volume=-1.0f,float pitch=1.0f);
	void stop();
	void pause();
	void resume();
	void set_loop(bool loop);
	void set_volume(float volume);
	float get_volime()const;
	bool is_playing()const;
	SoundEffect(const SoundEffect&) = delete;
	SoundEffect& operator=(const SoundEffect&) = delete;
private:
	bool is_voice_playing(IXAudio2SourceVoice* voice)const;
	std::vector<IXAudio2SourceVoice*>voices_{};
	std::vector<unsigned char>audio_data_{};
	WAVEFORMATEX wave_format_{};
	float volume_{ 1.0f };
	bool loop_{ false };
};

#endif