#ifndef GTLIB_TEXTURE_ID_H_
#define GTLIB_TEXTURE_ID_H_

#include "GTLibMath.h"

//テクスチャを用いるシステムを管理するID
class GTLibTextureID {
public:
	//デフォルトコンストラクタ
	GTLibTextureID(){
		initialize();
	};
	//デフォルトデストラクタ
	~GTLibTextureID() = default;
	//単一化
	static GTLibTextureID* Instance() {
		static GTLibTextureID t_id;
		return &t_id;
	}
	//初期化
	void initialize(GTUINT render_target_count=10, GTUINT depth_count =10, GTUINT textrue_count =1024);
	//各種値を返す
	//レンダーターゲットの最小値
	GTUINT render_target_id_min()const { return render_target_id_min_; }
	//レンダーターゲットの最大値
	GTUINT render_target_id_max()const { return render_target_id_max_; }
	//深度の最小値
	GTUINT depth_id_min()const { return depth_id_min_; }
	//深度の最大値
	GTUINT depth_id_max()const { return depth_id_max_; }
	//テクスチャの最少値
	GTUINT textrue_id_min()const { return textrue_id_min_; }
	//テクスチャの最大値
	GTUINT textrue_id_max()const { return textrue_id_max_; }
private:
	GTUINT render_target_id_min_{ 0 };//レンダーターゲットの最小値
	GTUINT render_target_id_max_{ 0 };//レンダーターゲットの最大値
	GTUINT depth_id_min_{ 0 };//深度の最小値
	GTUINT depth_id_max_{ 0 };//深度の最大値
	GTUINT textrue_id_min_{ 0 };//テクスチャの最少値
	GTUINT textrue_id_max_{ 0 };//テクスチャの最大値
};

#endif