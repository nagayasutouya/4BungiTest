#ifndef CSV_READER_H_
#define CSV_READER_H_

#include <vector>
#include<string>

class CSVReader {

public:

	CSVReader() = default;
	CSVReader(const std::string& file_name);

	void load(const std::string& file_name="",bool like_csv=true,bool creat=false);
	void save(const std::vector<std::string>& data={}, const std::string& name = "", const bool creat = true);
	void save(const std::vector<std::vector<std::string>>& data={}, const std::string& name="",const bool creat=true);
	const std::vector<std::vector<std::string>>get_rows()const;
	const std::vector<std::string>get_row(int row)const;
	const std::string& get(int row, int column)const;
	int geti(int row, int column)const;
	float getf(int row, int column)const;
	int rows()const;
	int columns(int row = 0)const;
	bool creat_folder(std::string forlder_name);//フォルダを生成
	void creat_path(std::string path);//パスを分解してフォルダ生成

private:

	using Row = std::vector<std::string>;
	using Rows = std::vector<Row>;

	Rows rows_;

};

#endif