#ifndef ASSETS_LOAD_H_
#define ASSETS_LOAD_H_

#include <string>
#include <type_traits>
#include "GTLib/Data/DefineVector.h"
#include "GTLib/Util/DiveineRow.h"
#include "GTLib/Util/SystemLog/CreatLog.h"
#include "GTLib/Data/AssetsStructs.h"
#include "GTLib/Util/AssetsManager/LoadPath.h"

namespace DefineAssetsLoad {
	using Category = std::vector<AssetsData>;
	using Genre = std::unordered_map<int,Category>;
	using All = std::unordered_map<std::string,Genre>;
}

//外部データからパス、ID、アセット名を読み込むクラス
class AssetsLoad {
public:
	AssetsLoad(){}
	static AssetsLoad* Instance(){
		static AssetsLoad al;
		return &al;
	}
	void load(std::string path);
	void clear() {
		load_path_.clear();
		used_assets_data_.clear();
		assets_data_.clear();//読み込んだアセットデータを分割、保存する場所
	};
	//データを返す
	std::unordered_map < std::string, DevideRow > assets_data()const { return assets_data_; }
	DevideRow assets_data(std::string genre)const { return assets_data_.at(genre); }
	Rows assets_data(std::string genre, int category){ return assets_data_.at(genre).get_rows(category); }
	LoadPath<std::string> load_path()const { return load_path_; }
	Row genre_name_list()const { return load_path_.genre_name_list(); }
	//外部から読み込んだデータからアセットIDを探す
	template<typename T>
	AssetsData find_data(std::string genre,const T assets_name ) {
		DevideRow dr = assets_data_.at(genre);
		AssetsData result = find<T>(genre,dr, assets_name);
		return result;
	}
	template<typename T>
	AssetsData find_data(const T assets_name) {
		std::vector<AssetsData> result;
		for (const auto& dr : assets_data_) {
			AssetsData data = find<T>(dr.first,dr.second, assets_name);
			if(data.name_!="")result.push_back(data);
		}
		return result;
	}

	//行毎に読み込む
	//1個だけ読み込む
	AssetsData find_data_with_row(std::string genre,int cutegory_id,int row_num) {
		AssetsData result = input(assets_data_.at(genre).get_row(cutegory_id, row_num), cutegory_id, genre);
		used_assets_data_.push_back(result);
		return result;
	}
	//1カテゴリーだけ読み込む
	DefineAssetsLoad::Category find_data_with_row(std::string genre, int cutegory_id) {
		DefineAssetsLoad::Category result;
		Rows data = assets_data(genre, cutegory_id);
		for (int i = 0; (unsigned)i < data.size(); i++) {
			result.push_back(find_data_with_row(genre, cutegory_id, i));
		}
		return result;
	}
	//1ジャンルだけ読み込む
	DefineAssetsLoad::Genre find_data_with_row(std::string genre){
		DefineAssetsLoad::Genre result;
		DevideRow data = assets_data(genre);
		for (const auto& ss : data.data()) {
			int catecory_id = ss.first;
			result.insert(std::pair<int, DefineAssetsLoad::Category>(catecory_id,find_data_with_row(genre,catecory_id)));
		}
		return result;
	}
	//全部読み込む
	DefineAssetsLoad::All find_data_with_row() {
		DefineAssetsLoad::All result;
		for (const auto& s : assets_data_) {
			std::string genre_id = s.first;
			result.insert(std::pair<std::string,DefineAssetsLoad::Genre>(genre_id,find_data_with_row(genre_id)));
		}
		return result;
	}
	//履歴を返す
	AssetsData used_assets_data(int pos)const {
		AssetsData data;
		if (used_assets_data_.size() > 0)data = used_assets_data_.at(pos);
		return data;
	}
	std::vector<AssetsData> used_assets_data()const { return used_assets_data_; }
	//履歴の中から対象のデータを探す
	template<typename T>
	AssetsData find_used_assets_data(const T target) {
		AssetsData result;
		for (int i = 0; (unsigned)i < used_assets_data_.data().size(); i++) {
			if (std::is_same<T, std::string>::value) {
				if (check(target, used_assets_data_.data().at(i).name_)) {
					result = used_assets_data_.data().at(i);
					break;
				}
			}
			else if (std::is_same<T, int>::value) {
				if (check(target, used_assets_data_.data().at(i).id_)) {
					result = used_assets_data_.data().at(i);
					break;
				}
			}
			else break;
		}
		return result;
	}

private:
	std::unordered_map < std::string, DevideRow > assets_data_;//読み込んだアセットデータを分割、保存する場所
	std::vector<AssetsData> used_assets_data_;
	LoadPath<std::string> load_path_;
private:
	//IDもしくはアセット名を元にサーチ
	template<typename T>
	AssetsData find(std::string genre_id,DevideRow data,const T target) {
		AssetsData result;
		int cutegory_id = -1;
		Rows rows;
		//データ全部を調べる
		for (const auto& rows_d : data.data()) {
			//カテゴリー別にサーチ
			cutegory_id = rows_d.first;
			rows = rows_d.second;
			for (int i = 0; (unsigned)i < rows.size(); i++) {
				Row row = rows.at(i);
				std::string from;
				if (std::is_same<T, std::string>::value) {
					from = row.at(0);
				}
				else if (std::is_same<T, int>::value) {
					from = row.at(1);
				}
				//一致したらやめる
				if (check(target,from)) {
					result = input(row,cutegory_id,genre_id);
					used_assets_data_.push_back(result);
					break;
				}
			}
		}
		return result;
	}
	//各種チェック
	bool check(std::string target, std::string from){
		return target == from;
	}
	bool check(float target, std::string from) {
		return target == stof(from);
	}
	bool check(int target, std::string from) {
		return target == stoi(from);
	}
	AssetsData input(Row row, int cutegory_id, std::string genre_id) {
		AssetsData result;
		result.name_ = row.at(0);
		result.id_ = stoi(row.at(1)) + cutegory_id;
		result.path_ = row.at(2);
		result.genre_id = genre_id;
		//改善ポイント
		if (genre_id == "Shader") {
			result.fragment_shader_name_ = row.at(3);
		}
		else if (genre_id == "SE") {
			result.play_count_= stoi(row.at(3));
			result.volume_=stoi(row.at(4));
		}
		else if (genre_id == "BGM") {
			result.loop_= (stoi(row.at(3))> 0)?true:false;
		}
		else if (genre_id == "Mesh") {
			result.skelton_path_ = row.at(3);
			result.animation_path_=row.at(4);
		}
		return result;
	}
};

#endif