#include"Windows.h"
#include"CSVReader.h"
#include<fstream>
#include<sstream>
#include<stdexcept>
#include <algorithm>
#include <iostream>
#include <direct.h>
#define MAKE_DIR(path) _mkdir(path)

CSVReader::CSVReader(const std::string& file_name) {
	load(file_name);
}
//外部からデータを読み込む
void CSVReader::load(const std::string& file_name, bool like_csv, bool creat) {
	//ないフォルダとかを生成
	if (creat) { creat_path(file_name); }
	std::ifstream file{ file_name };
	if (!file) {
		throw std::runtime_error("csvfile is not open");
	}
	rows_.clear();
	std::string line;
	while (getline(file, line)) {
		//区切り文字を増やして分解に段階を作る
		if(!like_csv)replace(line.begin(),line.end(),'_',',');
		std::stringstream ss{ line };
		std::string value;
		Row row;
		while (getline(ss, value, ',')) {
			row.push_back(value);
		}
		rows_.push_back(row);
	}
}
//一行を保存
void CSVReader::save(const std::vector<std::string>& data, const std::string& name, const bool creat) {
	std::ofstream ofs(name);
	if (!ofs.is_open()) {
		// エラー番号表示
		if(creat)std::cout << "CreatTextFileError : "<<name<< std::endl;
		return;
	}
	for (int i = 0; (unsigned)i < data.size(); i++) {
		ofs << data.at(i) << std::endl;
	}
	ofs.close();
}
//複数の行を保存
void CSVReader::save(const std::vector<std::vector<std::string>>& data, const std::string& name, const bool creat) {
	std::ofstream ofs(name);
	if (!ofs.is_open()&&!creat) {
		return;
	}
	for (int i = 0; (unsigned)i < data.size(); i++) {
		for (int j = 0; (unsigned)j < data.at(i).size(); j++) {
			ofs << data.at(i).at(j);
		}
		ofs<< std::endl;
	}
	ofs.close();
}
//全データを返す
const std::vector<std::vector<std::string>>CSVReader::get_rows()const{
	return rows_;
}
//行を返す
const std::vector<std::string>CSVReader::get_row(int row)const {
	return rows_.at(row);
}
//一のデータを返す
const std::string& CSVReader::get(int row, int column)const {
	return rows_[row][column];
}
//一つのデータをintで返す
int CSVReader::geti(int row, int column)const {
	return stoi(get(row, column));
}
//一つのデータをfloatで返す
float CSVReader::getf(int row, int column) const {
	return stof(get(row, column));
}
//行数を返す
int CSVReader::rows()const {
	return (int)rows_.size();
}
//一行に入っているデータ数を返す
int CSVReader::columns(int row)const {
	return (int)rows_[row].size();
}
//フォルダを生成
bool CSVReader::creat_folder(std::string folder_name){
	return MAKE_DIR(folder_name.c_str());
}
//パスを分解してフォルダ生成
void CSVReader::creat_path(std::string path){
	std::vector<std::string> paths;
	std::stringstream ss{ path };
	std::string value;
	//パスを階層ごとに分解
	while (getline(ss, value, '/')) {
		paths.push_back(value);
	}
	//無いやつだけ生成
	for (int i = 0; (unsigned)i < paths.size()-1; i++) {
		//フォルダの回想を設定
		std::string folder_path;
		for (int j = 0; j < i+1; j++) {
			folder_path += paths.at(j);
			if (j < i)folder_path += '/';
		}
		if (!creat_folder(folder_path)) {
			// エラー番号表示
			std::cout << "CreatFolderError : " << GetLastError() << std::endl;
		}
	}
	std::vector<std::string>data;
	save(data,path);
}