#ifndef LOAD_DEVIDE_H_
#define LOAD_DEVIDE_H_

#include "GTLib/Util/DiveineRow.h"
#include "LoadPath.h"

template<typename Type>
class LoadDevide {
public:
	LoadDevide(){}
	//外部保存してあるジャンル名とパスからサーチ＆ロード
	void load(std::string path_data,Type target) {
		CSVReader csv;
		//IDを元にフォルダの位置を取得
		int row_num = load_path_.load(path_data,  csv, target);
		//パスを元にデータ読み込み
		std::string path = csv.get(row_num, 1);
		//データ分割
		devide_row_.input(path, load_path_.ids(), load_path_.ids_add(), true);
	}
	LoadPath<Type> path()const { return load_path_; }
	DevideRow devide_row()const { return devide_row_; }
private:
	LoadPath<Type> load_path_;
	DevideRow devide_row_;
};

#endif