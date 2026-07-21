#include"SoundEffect.h"
#include"AudioFileReader.h"
#include<stdexcept>
#include<string>

#pragma comment(lib,"xaudio2.lib")

SoundEffect::SoundEffect(IXAudio2* xaudio2, const std::string& file_name, const SoundEffect::Desc& desc) :volume_{ desc.volume }, loop_{desc.loop} {
	AudioFileReader reader{ file_name };
	wave_format_ = reader.wave_format();
	reader.read_all_audio_sample(audio_data_);
	XAUDIO2_SEND_DESCRIPTOR send_desc{};
	XAUDIO2_VOICE_SENDS send_list{};
	if (desc.submix_voice) {
		send_desc.Flags = 0;
		send_desc.pOutputVoice = desc.submix_voice;
		send_list.SendCount = 1;
		send_list.pSends = &send_desc;
	}
	voices_.resize(desc.max_voices,nullptr);
	for (UINT i = 0; i < desc.max_voices; ++i) {
		HRESULT hr = xaudio2->CreateSourceVoice(&voices_[i], &wave_format_, 0, XAUDIO2_DEFAULT_FREQ_RATIO, nullptr, desc.submix_voice ? &send_list : nullptr, nullptr);
		if (FAILED(hr)) {
			for (IXAudio2SourceVoice* voice : voices_) {
				if (voice)voice->DestroyVoice();
			}
			throw std::runtime_error("Failed to creat source voice");
		}
		voices_[i]->SetVolume(volume_);
	}
}
SoundEffect::~SoundEffect() { clear(); }
void SoundEffect::clear(){
	stop();
	for (auto* voice : voices_) { voice->DestroyVoice(); }
	voices_.clear();
}
int SoundEffect::play(bool overlap, float volume , float pitch ){
	if (!overlap && is_playing())return -1;
	for (int i = 0; i < (int)voices_.size();++i) {
		if (is_voice_playing(voices_[i]))continue;//再生中のはスルー
		voices_[i]->Stop();
		voices_[i]->FlushSourceBuffers();
		voices_[i]->SetVolume(volume * volume_);
		voices_[i]->SetFrequencyRatio(pitch);
		XAUDIO2_BUFFER buffer{};
		buffer.AudioBytes = (UINT32)audio_data_.size();
		buffer.pAudioData = audio_data_.data();
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = loop_ ? XAUDIO2_LOOP_INFINITE : 0;
		voices_[i]->SubmitSourceBuffer(&buffer);
		voices_[i]->Start();
		return i;
	}
	return -1;//空きボイス無し
}
void SoundEffect::stop(){
	for (IXAudio2SourceVoice* voice : voices_) {
		voice->Stop();
		voice->FlushSourceBuffers();
	}
}
void SoundEffect::pause(){
	for (IXAudio2SourceVoice* voice : voices_) {
		voice->Stop();
	}
}
void SoundEffect::resume(){
	for (IXAudio2SourceVoice* voice : voices_) {
		if (is_voice_playing(voice)) {
			voice->Start();
		}
	}
}
void SoundEffect::set_loop(bool loop){
	loop_ = loop;
}
void SoundEffect::set_volume(float volume){
	volume_ = volume;
	for (IXAudio2SourceVoice* voice : voices_) {
		voice->SetVolume(volume_);
	}
}
float SoundEffect::get_volime()const { return volume_; }
bool SoundEffect::is_playing()const{
	for (IXAudio2SourceVoice* voice : voices_) {
		if (is_voice_playing(voice))return true;
	}
	return false;
}
bool SoundEffect::is_voice_playing(IXAudio2SourceVoice* voice)const{
	XAUDIO2_VOICE_STATE state{};
	voice->GetState(&state);
	return state.BuffersQueued > 0;
}