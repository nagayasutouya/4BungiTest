#ifndef DEVINE_ROW_H_
#define DEVINE_ROW_H_

#include <vector>
#include <string>
#include <unordered_map>
#include "GTLib/Util/CSVReader.h"

//読み込んだデータをジャンル毎に分割し保存するクラス 次使うときはunorderedをテンプレートにしたい
class DevideRow {

public:

	DevideRow() = default;
	DevideRow(std::unordered_map<int, std::vector<std::vector<std::string>>> data);

	int rows(int key);//一タイプが内包する行数
	int row(int key, int row);//一行のサイズ

	std::unordered_map<int, std::vector<std::vector<std::string>>> data()const { return data_; }
	std::vector<std::string> get_row(int key,int row);//１行を返す
	std::vector<std::vector<std::string>> get_rows(int key);//1タイプを返す
	int geti(int key, int row,int count);//int返す
	float getf(int key, int row, int count);//float返す
	std::string get(int key, int row, int count);//返す
	bool is_alive(int key);//キーに対応したコンテナがあるか

	void input(std::unordered_map<int, std::vector<std::vector<std::string>>> data);
	void input(std::string file_name, std::vector<std::string>load_data_id, std::vector<int>result_id_add, const bool devine_like_csv);

	std::unordered_map<int, std::vector<std::vector<std::string>>> devide_data_with_type(std::string file_name, std::vector<std::string>load_data_id, std::vector<int>result_id_add, const bool devine_like_csv);//データをタイプごとに分別
	int judge_data_type(std::string data, std::vector<std::string>load_data_id, std::vector<int>result_id_add);//データタイプを決める
	int add_ldt_id(int type);//idを追加する
	int ldt_id_size(int key);//
	
private:

	using Row = std::vector<std::string>;//一行
	using Rows = std::vector<Row>;//行で保存されたデータ群
	using DevineRows = std::unordered_map<int, Rows>;//種類ごとに保存されたデータ群

	DevineRows data_;
	CSVReader csv_;

	std::vector<int>ldt_id_add_;//ロードデータの数数えるやつ

};

#endif