#include"CreatLog.h"

CreatLog::~CreatLog() { save(); }
//ファイル名を生成する
std::string CreatLog::creat_file_name(std::string type){
	std::string data=current_time();
	data +=type + ".txt";
	return data;
}
//ログ出力方法を設定する
void CreatLog::set_propaty(std::string path, bool time,int max){
	csv_.load(path,true,true);
	path_ = path;
	time_flag_ = time;
	log_max_ = max;
	log_count_ = 0;
}
//ログを出力する
void CreatLog::creat_log(std::string log,bool time){
	std::string data;
	if (time) {
		char s[8];
		snprintf(s, sizeof(s), "%d", current_log_id_);
		data = s;
		data += ',';
		if (time_flag_)data += current_time(true) + ',';
		data += log;
	}
	else data = log;
	current_log_id_++;
	//上限付きで枠パンパンだったら上から書き換え
	if(log_max_>0&&(signed)log_.size()>=log_max_){
		log_.at(log_count_) = data;
		log_count_++;
		//最後まで行ったらリセット
		if (log_count_ >= log_max_)log_count_ = 0;
	}
	else if(log_max_<0)log_.push_back(data);
}
void CreatLog::creat_log_v3(std::string log , Vector3 data, bool time){
	char s[data_size_];
	snprintf(s, data_size_, ":%f,%f,%f", data.x, data.y, data.z);
	log += s;
	creat_log(log,time);
}
void CreatLog::creat_log_v2(std::string log , Vector2 data, bool time){
	char s[data_size_];
	snprintf(s, data_size_, ":%f,%f", data.x, data.y);
	log += s;
	creat_log(log, time);
}
void CreatLog::creat_log_f(std::string log , float data, bool time){
	char s[data_size_];
	snprintf(s, data_size_, ":%f", data);
	log += s;
	creat_log(log, time);
}
void CreatLog::creat_log_i(std::string log , int data, bool time){
	char s[data_size_];
	snprintf(s, data_size_, ":%d", data);
	log += s;
	creat_log(log, time);
}
//現在時間を取得
tm CreatLog::local_time(){
	return UtilLogFolder::local_time();
}
//年月日を返す
std::string CreatLog::current_day(){
	return UtilLogFolder::current_day();
}
//現在時間を返す
std::string CreatLog::current_time(bool ms){
	return UtilLogFolder::current_time(ms);
}
//現在のミリ秒を返す
std::chrono::milliseconds CreatLog::mil_sec(){
	return UtilLogFolder::mil_sec();
}
//保存
void CreatLog::save() {
	csv_.save(log_, path_);
}