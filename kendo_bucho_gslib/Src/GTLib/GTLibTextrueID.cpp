#include "GTLibTextrueID.h"

//初期化
void GTLibTextureID::initialize(GTUINT render_target_count, GTUINT depth_count, GTUINT textrue_count){
	render_target_id_min_ = 0;//レンダーターゲットの最小値
	render_target_id_max_ = render_target_count-1;//レンダーターゲットの最大値
	depth_id_min_= render_target_count;//深度の最小値
	depth_id_max_= depth_count-1+depth_id_min_;//深度の最大値
	textrue_id_min_= depth_id_max_+1;//テクスチャの最少値
	textrue_id_max_= textrue_count-1+textrue_id_min_;//テクスチャの最大値
}