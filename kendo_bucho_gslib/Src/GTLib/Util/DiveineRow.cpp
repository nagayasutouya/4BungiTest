#include"DiveineRow.h"
#include<stdexcept>

DevideRow::DevideRow(std::unordered_map<int, std::vector<std::vector<std::string>>> data):data_ {data}{}

//一タイプが内包する行数
int DevideRow::rows(int key) {
	return data_.at(key).size();
}

//一行のサイズ
int DevideRow::row(int key, int row) {
	return data_.at(key).at(row).size();
}

//１行を返す
std::vector<std::string> DevideRow::get_row(int key, int row) {
	return data_.at(key).at(row);
}

//1タイプを返す
std::vector<std::vector<std::string>> DevideRow::get_rows(int key) {
	return data_.at(key);
}

//返す
std::string DevideRow::get(int key, int row, int count){
	return data_.at(key).at(row).at(count);
}

//intで返す
int DevideRow::geti(int key, int row,int count){
	return stoi(data_.at(key).at(row).at(count));
}

//floatで返す
float DevideRow::getf(int key, int row, int count){
	return stof(data_.at(key).at(row).at(count));
}

//キーに対応したコンテナがあるか
bool DevideRow::is_alive(int key){
	bool result = false;
	if (data_.find(key) != data_.end())result = true;
	return result;
}

void DevideRow::input(std::unordered_map<int, std::vector<std::vector<std::string>>> data) {
	data_ = data;
}

void DevideRow::input(std::string file_name, std::vector<std::string>load_data_id, std::vector<int>result_id_add,const bool devine_like_csv){
	data_ = devide_data_with_type(file_name,load_data_id,result_id_add,devine_like_csv);
}

std::unordered_map<int, std::vector<std::vector<std::string>>> DevideRow::devide_data_with_type(std::string file_name,std::vector<std::string>load_data_id,std::vector<int>result_id_add, const bool devine_like_csv) {
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
int DevideRow::judge_data_type(std::string data, std::vector<std::string>load_data_id, std::vector<int>result_id_add) {
	int result = -1;
	for (int i = 0; (unsigned)i < load_data_id.size(); i++) {
		if (data == load_data_id.at(i)){
			result = result_id_add.at(i) + add_ldt_id(i); break;
		}
	}
	return result;
}

//データロードIDを追加する
int DevideRow::add_ldt_id(int type) {
	return ldt_id_add_.at(type)++;
}

int DevideRow::ldt_id_size(int key){
	return ldt_id_add_.at(key);
}