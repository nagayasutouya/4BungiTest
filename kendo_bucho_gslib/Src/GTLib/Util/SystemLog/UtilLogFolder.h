#ifndef DELETE_LOG_FOLDER_H_
#define DELETE_LOG_FOLDER_H_

#include<string>
#include <chrono>
#include <ctime>
//フォルダ管理に使う計算類
namespace UtilLogFolder{
	void destroy_folder_before(std::string path, int before);//指定日時より更新日が古いフォルダを削除
	void destroy_folder(std::string path="");//フォルダを削除
	std::string current_day();//年月日を返す
	std::string current_time(bool ms = false);//現在時間を返す
	tm local_time();//現在時間を取得
	std::chrono::milliseconds mil_sec();//現在のミリ秒を返す
};

#endif