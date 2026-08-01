#include"RowsManager.h"

RowsManager::RowsManager(std::vector<std::vector<std::string>> data):data_ {data}{}

//一タイプが内包する行数
int RowsManager::rows(){
	return data_.size();
}

//一行のサイズ
int RowsManager::row(int row) {
	return data_.at(row).size();
}

//１行を返す
std::vector<std::string> RowsManager::get_row(int row){
	return data_.at(row);
}

//データ全部返す
std::vector<std::vector<std::string>> RowsManager::data(){
	return data_;
}

//int返す
int RowsManager::geti(int row, int count){
	return stoi(data_.at(row).at(count));
}

//float返す
float RowsManager::getf(int row, int count){
	return stof(data_.at(row).at(count));
}

//返す
std::string RowsManager::get(int row, int count){
	return data_.at(row).at(count);
}

//新規コンテナ生成
void RowsManager::check_data_size(int row, int count, std::string data){
	if ((unsigned)row > data_.size()) {
		add_row();
	}
	if ((unsigned)count > data_.at(row).size()) {
		add_data(row, data);
	}
}

//データ追加
void RowsManager::add_data(int row, std::string data) {
	data_.at(row).push_back(data);
}

//行追加
void RowsManager::add_row(){
	Row new_row;
	data_.push_back(new_row);
}

//int代入
void RowsManager::seti(int row, int count, int data) {
	check_data_size(row,count,"0");
	data_.at(row).at(count) = std::to_string(data);
}

//float代入
void RowsManager::setf(int row, int count, float data) {
	check_data_size(row, count,"0");
	data_.at(row).at(count) = std::to_string(data);
}

//代入
void RowsManager::set(int row, int count, std::string data) {
	check_data_size(row, count,"a");
	data_.at(row).at(count) = data;
}

//キーに対応したコンテナがあるか
bool RowsManager::is_alive(int key){
	bool result =(data_.size()<(unsigned)key)?false:true;
	return result;
}

//指定した行の中身を削除
void RowsManager::clear_row(const int row){
	data_.at(row).clear();
}