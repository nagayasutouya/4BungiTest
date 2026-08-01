#ifndef CREAT_LOG_H_
#define CREAT_LOG_H_
#include <chrono>
#include <ctime>
#include "GTLib/Util/CSVReader.h"
#include "GTLib/Util/SystemLog/UtilLogFolder.h"

//ログを吐き出すクラス
class CreatLog {
public:
	CreatLog(){}
	~CreatLog();
	std::string creat_file_name(std::string type);//ファイル名を生成する
	void set_propaty(std::string path="",bool time=false,int max=0);//ログ出力方法を設定する
	void creat_log(std::string log="",bool time=true);//ログを出力する
	std::string current_day();//年月日を返す
	std::string current_time(bool ms=false);//現在時間を返す
	tm local_time();//現在時間を取得
	std::chrono::milliseconds mil_sec();//現在のミリ秒を返す
private:
	CSVReader csv_;
	std::string path_;
	bool time_flag_{ false };
	std::vector<std::string> log_;
	int current_log_id_{ 0 };
	int log_count_{ 0 };//ログを出力した回数
	int log_max_{ 0 };//ログを出力する最大数
};

#endif