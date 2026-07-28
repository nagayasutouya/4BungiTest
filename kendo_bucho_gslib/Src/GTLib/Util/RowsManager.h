#ifndef ROWS_MANAGER_H_
#define ROWS_MANAGER_H_

#include <string>
#include <vector>
//複数行のデータを簡単に扱うためのクラス
class RowsManager {

public:

	RowsManager()=default;
	RowsManager(std::vector < std::vector<std::string>> data);
	int rows();//一タイプが内包する行数
	int row(int row);//一行のサイズ

	std::vector<std::string> get_row(int row);//１行を返す
	std::vector<std::vector<std::string>> data();//データ全部返す
	int geti(int row, int count);//int返す
	float getf(int row, int count);//float返す
	std::string get(int row, int count);//返す
	void check_data_size(int row, int count, std::string data);//新規コンテナ生成
	void add_data(int row, std::string data);//データ追加
	void add_row();//行追加
	void seti(int row, int count, int data);//int代入
	void setf(int row, int count, float data);//float代入
	void set(int row, int count, std::string data);//代入
	bool is_alive(int key);//キーに対応したコンテナがあるか
	void clear_row(const int row);

private:

	using Row = std::vector<std::string>;//一行
	using Rows = std::vector<Row>;//行で保存されたデータ群
	Rows data_;

};

#endif