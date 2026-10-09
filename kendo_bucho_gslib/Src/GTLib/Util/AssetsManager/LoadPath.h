#ifndef LOAD_PATH_H_
#define LOAD_PATH_H_

#include <type_traits>
#include "GTLib/Data/DefineVector.h"
#include "GTLib/Util/CSVReader.h"
#include "GTLib/Util/SystemLog/CreatLog.h"

//ターゲットがどのパスを使用し、どのジャンルで分けるのかを外部から読み込んでくるクラス
template<typename Type>
class LoadPath {
public:
	LoadPath(){}
	int load(std::string path, CSVReader& csv, const Type target) {
		//IDを元にフォルダの位置を取得
		csv.load(path);
		int size = csv.rows();
		if (size <= 1) {
			return -1;
		}
		int col = csv.columns(0);
		for (int j = 0; j < col; j++) {
			ids_.push_back(csv.get(0, j));
			ids_add_.push_back(csv.geti(1, j));
		}
		int row_num = 2;
		//ジャンル名保存
		input_genre(row_num,csv);
		while (row_num < size) {
			//ターゲットがstring
			if (std::is_same<Type, std::string>::value ) { 
				if(is_correct(csv,row_num,target))break;
			}
			//ターゲットがint
			else if (std::is_same<Type, int>::value) {
				if(is_correct(csv, row_num, target))break;
			}
			//ターゲットがfloat
			else if (std::is_same<Type, float>::value) { 
				if(is_correct(csv, row_num, target))break;
			}
			else { 
				return -1;
			}
			row_num++;
		}
		if (row_num >= size) {
			return -1;
		}
		return row_num;
	}
	void clear() {
		ids_add_.clear();
		ids_.clear();
		genre_name_list_.clear();
	}
	VInt ids_add()const { return ids_add_; }
	int id_add(int key)const { return ids_add_.at(key); }
	Row ids()const { return ids_; }
	std::string id(int key)const { return ids_.at(key); };
	Row genre_name_list()const { return genre_name_list_; }
private:
	bool is_correct(CSVReader& csv,int row_num,std::string target) {
		return csv.get(row_num, 0) == target;
	}
	bool is_correct(CSVReader& csv, int row_num, int target) {
		return csv.geti(row_num, 0) == target;
	}
	bool is_correct(CSVReader& csv, int row_num, float target) {
		return csv.getf(row_num, 0) == target;
	}
	void input_genre(int row_num,CSVReader& csv) {
		for (int i = row_num; i < csv.rows(); i++) {
			genre_name_list_.push_back(csv.get(i,0));
		}
	}
private:
	VInt ids_add_;
	Row ids_;
	Row genre_name_list_;
};

#endif