#ifndef CREAT_LOG_H_
#define CREAT_LOG_H_
#include <chrono>
#include <ctime>
#include "GTLib/Util/CSVReader.h"
#include "GTLib/Util/SystemLog/UtilLogFolder.h"
#include "GTLib/GTLibMath.h"

//ログを吐き出すクラス
class CreatLog {
public:
	CreatLog(){}
	~CreatLog();
	static CreatLog* Instance() {
		static CreatLog* cl = new CreatLog{};
		return cl;
	}
	//ファイル名を生成する
	std::string creat_file_name(std::string type);
	//ログ出力方法を設定する
	void set_propaty(std::string path="",bool time=false,int max=0);
	//ログを出力する
	void creat_log(std::string log="",bool time=true);
	void creat_log_v3(std::string log = "",Vector3 data=Vector3{0.0f,0.0f,0.0f}, bool time = true);
	void creat_log_v2(std::string log = "",Vector2 data = Vector2{ 0.0f,0.0f }, bool time = true);
	void creat_log_f(std::string log = "",float data = 0.0f, bool time = true);
	void creat_log_i(std::string log = "", int data = 0, bool time = true);
	//年月日を返す
	std::string current_day();
	//現在時間を返す
	std::string current_time(bool ms=false);
	//現在時間を取得
	tm local_time();
	//現在のミリ秒を返す
	std::chrono::milliseconds mil_sec();
	//保存
	void save();
public:
	const std::string LogFilePath{ "LocalLog/" };
private:
	CSVReader csv_;
	std::string path_;
	bool time_flag_{ false };
	std::vector<std::string> log_;
	int current_log_id_{ 0 };
	int log_count_{ 0 };//ログを出力した回数
	int log_max_{ 0 };//ログを出力する最大数]
	static const int data_size_{ 512 };
};

#endif