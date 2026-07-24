#ifndef MORTON_ORDER_H_
#define MORTON_ORDER_H_

#include"GTLib/GTLib.h"

typedef unsigned short GTWORD;
typedef unsigned short GTDWORD;

class QuadTree;

struct Short2 {
	GTWORD x = 0;
	GTWORD y = 0;
};

template<class T>
struct QuadTreeObj {
	QuadTree* space=nullptr;//空間
	T* obj = nullptr;//本体
	QuadTreeObj* p_obj = nullptr;//前
	QuadTreeObj* n_obj = nullptr;//次
};

class MotornOrder {
public:
	MotornOrder(){}
	MotornOrder(float space_size_left, float space_size_top, float space_size_right, float space_size_bottom){
		set_space_size(space_size_left, space_size_top,space_size_right,space_size_bottom);
	}
	MotornOrder(Vector2 space_left_top, Vector2 space_right_botton) {
		set_space_size(space_left_top,space_right_botton);
	}
	MotornOrder(Rect space_size) {
		set_space_size(space_size);
	}
	~MotornOrder(){}	
	static MotornOrder* instance() {
		static MotornOrder* mo = new MotornOrder();
		return mo;
	}
	int morton_order(Rect pos);//モートン序列順に分割した空間のどこにあたるか
	void set_space_size(Rect space_size);//空間のサイズを登録
	void set_space_size(float space_size_left, float space_size_top, float space_size_right, float space_size_bottom) { set_space_size(Rect{ space_size_left, space_size_top,space_size_right,space_size_bottom }); }//空間のサイズを登録
	void set_space_size(Vector2 space_left_top, Vector2 space_right_botton) { set_space_size(Rect{ space_left_top.x, space_left_top.y, space_right_botton.x, space_right_botton.y }); }//空間のサイズを登録
private:
	//点単位
	GTDWORD morton_num(GTWORD x, GTWORD y);//空間番号を出す
	GTDWORD morton_num(Short2 pos) { return morton_num(pos.x, pos.y); }//空間番号を出す
	GTDWORD bit_shift(GTWORD n);//空間番号出すためのbit演算
	//四角単位
	unsigned int object_in_morton_num(Short2 left_top, Short2 right_bottom);//オブジェクトがどのレイヤーのどの空間に属すか
private:
	Rect space_size_{0.0f,0.0f,0.0f,0.0f};//空間の大きさ
	Vector2 side_length_{ 0.0f,0.0f };//最小空間の辺のサイズ
};

#endif