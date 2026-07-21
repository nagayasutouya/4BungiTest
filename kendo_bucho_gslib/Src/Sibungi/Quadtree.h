#ifndef QUADTREE_H_
#define QUADTREE_H_

#include <list>
using namespace std;
class Actor;

typedef unsigned short GTWORD;
typedef unsigned short GTDWORD;

struct Short2 {
	GTWORD x = 0;
	GTWORD y = 0;
};

template<class T>
struct QuadTreeObj {
	int space_num=0;//空間番号
	T* obj;//本体
	QuadTreeObj* p_obj;//前
	QuadTreeObj* n_obg;//次
};

class QuadTree {
public:
	QuadTree(){}
	~QuadTree();
	void morton_order(float x, float y);//空間をモートン序列順に分割する
private:
	//点単位
	GTDWORD morton_num(GTWORD x, GTWORD y);//空間番号を出す
	GTDWORD morton_num(Short2 pos) { return morton_num(pos.x, pos.y); }//空間番号を出す
	GTDWORD bit_shift(GTWORD n);//空間番号出すためのbit演算
	//四角単位
	unsigned int object_in_morton_num(Short2 left_top, Short2 right_bottom);//オブジェクトがどのレイヤーのどの空間に属すか
	//n乗
	int nth_power(int target, int n)const;
private:
};

#endif