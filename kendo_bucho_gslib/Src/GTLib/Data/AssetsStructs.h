#ifndef ASSETS_STRUCTS_H_
#define ASSETS_STRUCTS_H_

#include "GTLib/GTLib.h"
#include <string>

const struct TextureData {
	unsigned int texture_id_ = 0;
	Vector2 position_ = { 0.0f,0.0f };
	Rect rect_ = { 0.0f,0.0f,0.0f,0.0f };
	Vector2 center_ = { 0.0f,0.0f };
	Color color_ = { 1.0f,1.0f,1.0f,1.0f };
	Vector2 scale_ = { 1.0f,1.0f };
	float angle_ = 0.0f;
};
const struct StringData {
	Vector2 position_ = { 0.0f,0.0f };//描画座標補正分
	int font_size_ = 0;//文字サイズ
	int font_field_ = 0; //文字コード保存先
	std::string font_id_ = "MSゴシック";//文字コード
	float arfa_speed_ = 0.0f;//点滅スピード
	Color color_ = { 0.0f,0.0f,0.0f,1.0f };//色
	unsigned int max_size_ = 9;//最大入力文字数
};
//外部アセットデータ読み込み用データ
const struct AssetsData {
	std::string name_ = "";
	int id_ = -1;
	std::string path_ = "";
	std::string genre_id = "";

	//スキニングメッシュ用
	std::string skelton_path_ = "";
	std::string animation_path_ = "";

	//シェーダー用
	std::string fragment_shader_name_ = "";

	//BGM
	bool loop_ = false;

	//SE
	int play_count_ = 0;
	int volume_ = 0;
};
#endif