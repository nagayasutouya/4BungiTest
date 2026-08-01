#include "AssetsLoad.h"
#include "GTLib/Util/CSVReader.h"

void AssetsLoad::load(std::string path,CreatLog& log) {
	CSVReader csv;
	load_path_.load(path, log, csv, "Nothing");
	//パスを元にデータ読み込み
	for (int i = 2; i < csv.rows(); i++) {
		std::string path = csv.get(i, 1);
		DevineRow data;
		data.input(path, load_path_.ids(), load_path_.ids_add(), true);
		//読み込んだデータを保存
		assets_data_.insert(std::pair<std::string, DevineRow>(csv.get(i, 0), data));
	}
}