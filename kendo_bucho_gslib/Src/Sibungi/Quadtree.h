#ifndef QUADTREE_H_
#define QUADTREE_H_

#include <vector>
#include "QuadTreeObj.h"
#include "MotornOrder.h"
#include "GTLib/GTLib.h"
class Actor;
using namespace std;

class QuadTreeSpace {
public:
	QuadTreeSpace(int id = 0);
	~QuadTreeSpace();
	void add_obj(QuadTreeObj<Actor>* add);//空間に追加
	bool removed(QuadTreeObj<Actor>* target);//離脱通知の受け取り
	void clear();//データ削除
	int space_id()const { return space_id_; }//空間番号
	QuadTreeObj<Actor>* first_object()const { return first_object_; }//先頭のオブジェクト
	void first_object(QuadTreeObj<Actor>* change) { first_object_= change; }//先頭のオブジェクト変更
	QuadTreeObj<Actor>* last_object()const { return last_object_; }//末尾のオブジェクト
	void last_object(QuadTreeObj<Actor>* change) { last_object_ = change; }//末尾のオブジェクト変更
private:
	QuadTreeObj<Actor>* first_object_{ nullptr };//先頭
	QuadTreeObj<Actor>* last_object_{ nullptr };//末尾
	int space_id_{ 0 };//空間の番号
};

#endif