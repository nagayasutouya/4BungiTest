#include "BindPathAndAssetsID.h"
#include "AssetsLoad.h"
#include "GTLib/Util/SystemLog/CreatLog.h"
#include "GTLib/GTLib.h"

void LoadOriginData(std::string path, CreatLog& log) {
	//アセットデータのパス読み込み
	AssetsLoad::Instance()->load(path, log);
}

void BindPathAndAssetsID::LoadGenre(std::string genre_name) {
	Row genre_list = AssetsLoad::Instance()->genre_name_list();
	for (const auto& genre : AssetsLoad::Instance()->find_data_with_row(genre_name)) {
		for (const auto& category : genre.second) {
			Load(genre_name, genre_list, category);
		}
	}
}
//ジャンルを指定してロードする
void BindPathAndAssetsID::LoadRow(std::string genre, int cutegory_id, int row_num_first, int row_num_end) {
	Row genre_list = AssetsLoad::Instance()->genre_name_list();
	for (int i = row_num_first; i <= row_num_end; i++) {
		AssetsData data = AssetsLoad::Instance()->find_data_with_row(genre, cutegory_id, i);
		Load(genre, genre_list, data);
	}
}
void BindPathAndAssetsID::LoadAll() {
	Row genre_list = AssetsLoad::Instance()->genre_name_list();
	for (int i = 0; i < (signed)genre_list.size(); i++) {
		std::string  genre_name = genre_list.at(i);
		for (const auto& genre : AssetsLoad::Instance()->find_data_with_row(genre_name)) {
			for (const auto& category : genre.second) {
				Load(genre_name, genre_list, category);
			}
		}
	}
}
//gsLoadする
void BindPathAndAssetsID::Load(std::string genre, Row genre_list, AssetsData data) {
	if (data.path_ == "")return;
	//ロード部分対応分増やす　あとAssetsDataの構造体にBGM SEに対応できるようにする
	if (genre == genre_list.at(0))Graphics::load_skinned_mesh(data.id_, data.path_.c_str(),data.skelton_path_,data.animation_path_);
	//else if (genre == genre_list.at(1))gsLoadOctree(data.id_, data.path_.c_str());
	//else if (genre == genre_list.at(2))gsLoadEffect(data.id_, data.path_.c_str());
	else if (genre == genre_list.at(3))Sound::load_bgm(data.id_, data.path_.c_str(), data.loop_);
	else if (genre == genre_list.at(4))Sound::load_se(data.id_, data.path_.c_str(), data.play_count_, data.volume_);
	else if (genre == genre_list.at(5))Graphics::load_texture (data.id_, data.path_.c_str());
	//else if (genre == genre_list.at(6))gsLoadShader(data.id_, data.path_.c_str(), data.fragment_shader_name_.c_str());
	//else if (genre == genre_list.at(7))gsLoadReflectionProbe(data.id_, data.path_.c_str());
	//else if (genre == genre_list.at(8))gsLoadLightmap(data.id_, data.path_.c_str());
	else return;
}
//ジャンルを指定して削除する
void BindPathAndAssetsID::DeleteGenre(std::string genre_name) {
	Row genre_list = AssetsLoad::Instance()->genre_name_list();
	for (const auto& genre : AssetsLoad::Instance()->find_data_with_row(genre_name)) {
		for (const auto& category : genre.second) {
			Delete(genre_name, genre_list, category);
		}
	}
}
void BindPathAndAssetsID::DeleteRow(std::string genre, int cutegory_id, int row_num_first, int row_num_end) {
	Row genre_list = AssetsLoad::Instance()->genre_name_list();
	for (int i = row_num_first; i <= row_num_end; i++) {
		AssetsData data = AssetsLoad::Instance()->find_data_with_row(genre, cutegory_id, i);
		Delete(genre, genre_list, data);
	}
}
void BindPathAndAssetsID::DeleteAll() {
	Row genre_list = AssetsLoad::Instance()->genre_name_list();
	for (int i = 0; i < (signed)genre_list.size();i++) {
		std::string  genre_name= genre_list.at(i);
		for (const auto& genre : AssetsLoad::Instance()->find_data_with_row(genre_name)) {
			for (const auto& category : genre.second) {
				Delete(genre_name, genre_list, category);
			}
		}
	}
}
//一部無視してのこりを消す
void BindPathAndAssetsID::DeleteIgnore(std::vector<std::string> ignore_list) {
	Row genre_list = AssetsLoad::Instance()->genre_name_list();
	for (int i = 0; i < (signed)genre_list.size(); i++) {
		std::string  genre_name = genre_list.at(i);
		//無視リストはスキップ
		bool skip = false;
		for (unsigned int k = 0; k < ignore_list.size(); k++) {
			if (ignore_list.at(k) == genre_name)
				skip=true;
		}
		if (skip)continue;
		for (const auto& genre : AssetsLoad::Instance()->find_data_with_row(genre_name)) {
			for (const auto& category : genre.second) {
				Delete(genre_name, genre_list, category);
			}
		}
	}
}
//gsDeleteする
void BindPathAndAssetsID::Delete(std::string genre, Row genre_list, AssetsData data) {
	if (data.path_ == "")return;
	//ロード部分対応分増やす　あとAssetsDataの構造体にBGM SEに対応できるようにする
	if (genre == genre_list.at(0))Graphics::delete_skinned_mesh(data.id_);
	//else if (genre == genre_list.at(1))gsDeleteOctree(data.id_);
	//else if (genre == genre_list.at(2))gsDeleteEffect(data.id_);
	else if (genre == genre_list.at(3)|| genre == genre_list.at(4))Sound::delete_sound_effect(data.id_);
	else if (genre == genre_list.at(5))Graphics::delete_texture(data.id_);
	//else if (genre == genre_list.at(6))gsDeleteShader(data.id_);
	//else if (genre == genre_list.at(7))gsDeleteReflectionProbe(data.id_);
	//else if (genre == genre_list.at(8))gsDeleteLightmap(data.id_);
	else return;
}