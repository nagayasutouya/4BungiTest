#ifndef BIND_PATH_AND_ASSETS_ID_H_
#define BIND_PATH_AND_ASSETS_ID_H_

#include "GTLib/Data/AssetsStructs.h"
#include <vector>
#include <string>
class  CreatLog;
//ロードされたアセットパスをもとにアセットをロードする
class BindPathAndAssetsID {
public:
	BindPathAndAssetsID(){}
	static BindPathAndAssetsID* Instance() {
		static BindPathAndAssetsID al;
		return &al;
	}
	//汎用
	void LoadOriginData(std::string path, CreatLog& log);
	void LoadGenre(std::string genre);//ジャンルを指定してロードする
	void LoadRow(std::string genre, int cutegory_id, int row_num_first, int row_num_end);//ジャンルを指定してロードする
	void LoadAll();//全部読み込む
	void Load(std::string genre, std::vector<std::string> genre_list, AssetsData data);//gsLoadする
	void DeleteGenre(std::string genre);//ジャンルを指定してロードする
	void DeleteRow(std::string genre, int cutegory_id, int row_num_first, int row_num_end);//ジャンルを指定してロードする
	void Delete(std::string genre, std::vector<std::string> genre_list, AssetsData data);//gsDeleteする
	void DeleteAll();//全部消す
	void DeleteIgnore(std::vector<std::string> ignore_list);
};
#endif