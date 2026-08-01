#include"DiveineRow.h"
#include<stdexcept>

DevineRow::DevineRow(std::unordered_map<int, std::vector<std::vector<std::string>>> data):data_ {data}{}

//一タイプが内包する行数
int DevineRow::rows(int key) {
	return data_.at(key).size();
}

//一行のサイズ
int DevineRow::row(int key, int row) {
	return data_.at(key).at(row).size();
}

//１行を返す
std::vector<std::string> DevineRow::get_row(int key, int row) {
	return data_.at(key).at(row);
}

//1タイプを返す
std::vector<std::vector<std::string>> DevineRow::get_rows(int key) {
	return data_.at(key);
}

//返す
std::string DevineRow::get(int key, int row, int count){
	return data_.at(key).at(row).at(count);
}

//intで返す
int DevineRow::geti(int key, int row,int count){
	return stoi(data_.at(key).at(row).at(count));
}

//floatで返す
float DevineRow::getf(int key, int row, int count){
	return stof(data_.at(key).at(row).at(count));
}

//キーに対応したコンテナがあるか
bool DevineRow::is_alive(int key){
	bool result = false;
	if (data_.find(key) != data_.end())result = true;
	return result;
}

void DevineRow::input(std::unordered_map<int, std::vector<std::vector<std::string>>> data) {
	data_ = data;
}

void DevineRow::input(std::string file_name, std::vector<std::string>load_data_id, std::vector<int>result_id_add,const bool devine_like_csv){
	data_ = devide_data_with_type(file_name,load_data_id,result_id_add,devine_like_csv);
}

std::unordered_map<int, std::vector<std::vector<std::string>>> DevineRow::devide_data_with_type(std::string file_name,std::vector<std::string>load_data_id,std::vector<int>result_id_add, const bool devine_like_csv) {
	DevineRows result;
	ldt_id_add_.clear();
	for (int i = 0; (unsigned)i < load_data_id.size(); i++) {
		ldt_id_add_.push_back(0);
	}
	csv_.load(file_name,devine_like_csv);
	//データを分別
	for (int i = 0; i < csv_.rows(); i++) {
		int j = judge_data_type(csv_.get(i, 0), load_data_id,result_id_add);
		if (j < 0)throw std::runtime_error("this file need defalent rule");
		i++;
		Rows input_row;
		//データタイプ変更フラグが来るまで
		while (csv_.get(i, 0) != "#end" && j >= 0) {
			input_row.push_back(csv_.get_row(i));
			i++;
		}
		result.insert(std::pair<int, Rows>(j, input_row));
	}
	return result;
}

//データタイプを決める
int DevineRow::judge_data_type(std::string data, std::vector<std::string>load_data_id, std::vector<int>result_id_add) {
	int result = -1;
	for (int i = 0; (unsigned)i < load_data_id.size(); i++) {
		if (data == load_data_id.at(i)){
			result = result_id_add.at(i) + add_ldt_id(i); break;
		}
	}
	return result;
}

//データロードIDを追加する
int DevineRow::add_ldt_id(int type) {
	return ldt_id_add_.at(type)++;
}

int DevineRow::ldt_id_size(int key){
	return ldt_id_add_.at(key);
}