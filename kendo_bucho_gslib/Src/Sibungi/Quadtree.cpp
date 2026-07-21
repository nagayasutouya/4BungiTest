#include"Quadtree.h"

QuadTree::~QuadTree(){}

//空間をモートン序列順に分割する
void QuadTree::morton_order(float x,float y){
	Short2 t1,t2;
	t1.x = 5;
	t1.y = 1;
	t2.x = 7;
	t2.y = 3;
	//t1.x = 1;
	//t1.y = 1;
	//t2.x = 7;
	//t2.y = 7;
	//t1.x = 2;
	//t1.y = 6;
	//t2.x = 3;
	//t2.y = 7;
	//t1.x = 0;
	//t1.y = 0;
	//t2.x = 0;
	//t2.y = 0;
	int res = object_in_morton_num(t1, t2);
	printf("Num:%d\n",res);
}

//空間番号を出す
GTDWORD QuadTree::morton_num(GTWORD x, GTWORD y) {
	return (bit_shift(x) | (bit_shift(y) << 1));
}

//空間番号出すためのbit演算
GTDWORD QuadTree::bit_shift(GTWORD n) {
	//16bitを32bitに変換
	n = (n | (n << 8)) & 0x00ff00ff;
	n = (n | (n << 4)) & 0x0f0f0f0f;
	n = (n | (n << 2)) & 0x33333333;
	return (n | (n << 1)) & 0x55555555;
}

//オブジェクトがどのレイヤーのどの空間に属すか
unsigned int QuadTree::object_in_morton_num(Short2 left_top, Short2 right_bottom){
	//オブジェクトが干渉している左上の空間を出す
	GTDWORD lt = morton_num(left_top.x,left_top.y);
	//オブジェクトが干渉している右下の空間を出す
	GTDWORD rb = morton_num(right_bottom.x, right_bottom.y);
	//上記の排他的論理和からオブジェクトが所属する空間を出す
	GTDWORD XOR = lt ^ rb;
	printf("LT:%d,RB:%d,XOR:%d\n", lt,rb,XOR);
	GTDWORD result = 0;
	int shift_count = 0;
	for ( int i= 3; i > 0;i--) {
		//チェックする2bitを右に押しやる
		result = (XOR>>((i-1)*2))&0b11;
		//０じゃなければ所属するレイヤーになる
		if (result > 0) {
			shift_count = i;
			break;
		}
	}
	//上位レイヤーから下位レイヤーまで順に割り振った所属空間で番号を出す
	//レイヤー番号:3,孫 2,子 1,親 0,全体
	if (shift_count == 3)return 0;//全体なら計算省く
	int morton_num = (rb >> (shift_count * 2));//レイヤー分け状態の番号出す
	printf("%d,%d\n", shift_count, morton_num);
	int layer_add=nth_power(4,shift_count);
	morton_num += (layer_add-1)/3;//レイヤー分けをなくす
	return morton_num;
}

int QuadTree::nth_power(int target, int n)const {
	int result = target;
	int check = (n-(target-1))*-1;
	for (int i = 1; i < check; i++)result *= target;
	return result;
}