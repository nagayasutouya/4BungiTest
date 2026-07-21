#include"AudioFileReader.h"
#include<propvarutil.h>
#include<stdexcept>
#include<filesystem>

#pragma comment(lib,"mf.lib")
#pragma comment(lib,"mfplat.lib")
#pragma comment(lib,"mfreadwrite.lib")
#pragma comment(lib,"mfuuid.lib")

AudioFileReader::AudioFileReader(const std::string& audio_file_path){
	std::filesystem::path file_path{audio_file_path};
	//オーディオのパスを設定 (LPCWSTR)file_path.u16string().c_str()これによりあらゆるstringに対応できる
	HRESULT hr = MFCreateSourceReaderFromURL((LPCWSTR)file_path.u16string().c_str(), nullptr, &source_reader_);
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to creat source reader");
	}

	//何のデータを読み込むかを決める
	IMFMediaType* media_type{};
	MFCreateMediaType(&media_type);
	media_type->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	media_type->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	source_reader_->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, media_type);
	media_type->Release();

	//波形データのフォーマットを取得
	source_reader_->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &media_type);
	WAVEFORMATEX* format{};
	MFCreateWaveFormatExFromMFMediaType(media_type, &format, nullptr, 0);
	wave_format_ = *format;//メディアタイプを一度削除しないといけないので、取ってきたフォーマットを保存しておく
	CoTaskMemFree(format);
	media_type->Release();
}

AudioFileReader::~AudioFileReader(){
	if (source_reader_ != nullptr) {
		source_reader_->Release();
	}
}

//単一読み込み
bool AudioFileReader::read_audio_sample(std::vector<unsigned char>& audio_buffer){
	//データを読み込み
	DWORD flags{};
	IMFSample* sample{};
	source_reader_->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &flags, nullptr, &sample);
	//データなかったら終了
	if ((flags & MF_SOURCE_READERF_ENDOFSTREAM) || sample == nullptr) {
		end_of_stream_ = true;
		audio_buffer.clear();
		return false;
	}
	IMFMediaBuffer* buffer{};
	sample->ConvertToContiguousBuffer(&buffer);
	BYTE* audio_data{};
	DWORD cur_length{};
	DWORD max_length{};
	buffer->Lock(&audio_data, &max_length, &cur_length);
	audio_buffer.resize(cur_length);
	//データをコピー
	std::memcpy(audio_buffer.data(), audio_data, cur_length);
	buffer->Unlock();
	buffer->Release();
	sample->Release();
	return true;
}

//全読み込み
void AudioFileReader::read_all_audio_sample(std::vector<unsigned char>& audio_buffer){
	audio_buffer.clear();
	std::vector<unsigned char>temp_buffer;
	while (read_audio_sample(temp_buffer)) {
		//読み込んだやつを追加
		audio_buffer.insert(audio_buffer.end(),temp_buffer.begin(), temp_buffer.end());
	}

}

//読み込み位置をリセット
void AudioFileReader::reset_position(LONGLONG position ){
	PROPVARIANT var{};
	InitPropVariantFromInt64(position, &var);
	source_reader_->SetCurrentPosition(GUID_NULL, var);
	PropVariantClear(&var);
	end_of_stream_ = false;
}

//ウェーブフォーマット
const WAVEFORMATEX& AudioFileReader::wave_format()const { return wave_format_; }

//ストリーミングが終わったか
bool AudioFileReader::end_of_stream()const { return end_of_stream_; }