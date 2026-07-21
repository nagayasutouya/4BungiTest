#ifndef AUDIO_FILE_READER_H_
#define AUDIO_FILE_READER_H_

#include<mfapi.h>
#include<mfidl.h>
#include<mfreadwrite.h>

#include<vector>
#include<string>

class AudioFileReader {
public:
	AudioFileReader(const std::string& audio_file_path);
	~AudioFileReader();
	bool read_audio_sample(std::vector<unsigned char>& audio_buffer);//単一読み込み
	void read_all_audio_sample(std::vector<unsigned char>& audio_buffer);//全読み込み
	void reset_position(LONGLONG position = 0);//読み込み位置をリセット
	const WAVEFORMATEX& wave_format()const;//ウェーブフォーマット
	bool end_of_stream()const;//ストリーミングが終わったか
private:
	IMFSourceReader* source_reader_{};
	WAVEFORMATEX wave_format_{};
	bool end_of_stream_{ false };
};
#endif