#include"UtilLogFolder.h"
#include<io.h>
#include<cstdlib>
#include <direct.h>

namespace UtilLogFolder {
	//指定日時より更新日が古いフォルダを削除
	void destroy_folder_before(std::string path, int before) {
		_finddata_t data;
		char c_path[256];
		if (_getcwd(c_path, sizeof(c_path)) != nullptr) {
			std::string folder = c_path;
			folder +="\\" + path;
			std::string f = folder;
			f += '*';
			intptr_t handle = _findfirst(f.c_str(), &data);
			if (handle == -1)return;
			time_t now = time(nullptr);
			do {
				//自身と親を除外
				if (strcmp(data.name, ".") == 0 || strcmp(data.name, "..") == 0)continue;
				// フォルダ判定
				if (data.attrib & _A_SUBDIR) {
					double diff = difftime(now, data.time_write);
					double days = diff / (60 * 60 * 24);
					//古ければ削除
					if (days >= before) { destroy_folder(folder + data.name); }
				}
			}
			//ファイルがほかにある間
			while (_findnext(handle, &data) == 0);
		}
	}
	//フォルダを削除
	void destroy_folder(std::string path){
		//削除コマンド
		std::string p = "rmdir /s /q ";
		p += path;
		system(p.c_str());
	}
	//現在時間を取得
	tm local_time() {
		auto now = std::chrono::system_clock::now();
		time_t now_c = std::chrono::system_clock::to_time_t(now);
		tm local;
		localtime_s(&local, &now_c);
		return local;
	}
	//年月日を返す
	std::string current_day() {
		tm lt = local_time();
		std::string data;
		char buf[512];
		strftime(buf, sizeof(buf), "%Y_%m_%d_", &lt);
		data = buf;
		return data;
	}
	//現在時間を返す
	std::string current_time(bool ms) {
		tm lt = local_time();
		std::string data;
		char buf[512];
		strftime(buf, sizeof(buf), "%H_%M_%S_", &lt);
		data = buf;
		if (ms)data += std::to_string(mil_sec().count());
		return data;
	}
	//現在のミリ秒を返す
	std::chrono::milliseconds mil_sec() {
		auto now = std::chrono::system_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
	}
};