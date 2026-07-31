#include "MotornOrder.h"

//モートン序列順に分割した空間のどこにあたるか
int MotornOrder::morton_order(Rect pos){
	//空間の範囲外なら-1を返す
	if (pos.right<space_size_.left||pos.bottom>space_size_.top||pos.left>space_size_.right||pos.top<space_size_.bottom)return -1;
	//空間番号割り振り用に正規化
	Short2 t1, t2;
	t1.x = (GTWORD)((pos.left - space_size_.left) / side_length_.x);
	t1.y = (GTWORD)((pos.top-space_size_.top) / side_length_.y);
	t2.x = (GTWORD)((pos.right - space_size_.left) / side_length_.x);
	t2.y = (GTWORD)((pos.bottom - space_size_.top) / side_length_.y);
	//部分的にでも入ってた用のCLAMP
	int s_min = 0, s_max = 7;
	t1.x = Mathf::CLAMP(t1.x, s_min, s_max);
	t1.y = Mathf::CLAMP(t1.y, s_min, s_max);
	t2.x = Mathf::CLAMP(t2.x, s_min, s_max);
	t2.y = Mathf::CLAMP(t2.y, s_min, s_max);
	return object_in_morton_num(t1, t2);
}
//空間番号を出す
GTDWORD MotornOrder::morton_num(GTWORD x, GTWORD y) {
	return (bit_shift(x) | (bit_shift(y) << 1));
}
//空間番号出すためのbit演算
GTDWORD MotornOrder::bit_shift(GTWORD n) {
	//16bitを32bitに変換
	n = (n | (n << 8)) & 0x00ff00ff;
	n = (n | (n << 4)) & 0x0f0f0f0f;
	n = (n | (n << 2)) & 0x33333333;
	return (n | (n << 1)) & 0x55555555;
}
//オブジェクトがどのレイヤーのどの空間に属すか
unsigned int MotornOrder::object_in_morton_num(Short2 left_top, Short2 right_bottom) {
	//オブジェクトが干渉している左上の空間を出す
	GTDWORD lt = morton_num(left_top.x, left_top.y);
	//オブジェクトが干渉している右下の空間を出す
	GTDWORD rb = morton_num(right_bottom.x, right_bottom.y);
	//上記の排他的論理和からオブジェクトが所属する空間を出す
	GTDWORD XOR = lt ^ rb;
	GTDWORD result = 0;
	int shift_count = 0;
	const int layer_num = 3;
	for (int i = layer_num; i > 0; i--) {
		//チェックする2bitを右に押しやる
		result = (XOR >> ((i - 1) * 2)) & 0b11;
		//０じゃなければ所属するレイヤーになる
		if (result > 0) {
			shift_count = i;
			break;
		}
	}
	//上位レイヤーから下位レイヤーまで順に割り振った所属空間で番号を出す
	//レイヤー番号:3,孫 2,子 1,親 0,全体
	//shiftからレイヤーへ変更
	int layer = (shift_count - layer_num) * -1;
	if (shift_count == layer_num)return 0;//全体なら計算省く
	int morton_num = (rb >> (shift_count * 2));//レイヤー分け状態の番号出す
	int layer_add = Mathf::nth_power(4, layer);
	morton_num += (layer_add - 1) / 3;//レイヤー分けをなくす
	return morton_num;
}
//空間のサイズを登録
void MotornOrder::set_space_size(Rect space_size){
	space_size_ = space_size;
	//フィールドサイズから最小空間の辺を出す
	const int space_count = 8;
	side_length_.x = (space_size_.right - space_size_.left) / space_count;
	side_length_.y = (space_size_.bottom - space_size_.top) / space_count;
}