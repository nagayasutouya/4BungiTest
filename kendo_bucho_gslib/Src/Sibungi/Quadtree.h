#ifndef QUADTREE_H_
#define QUADTREE_H_

#include <vector>
#include "MotornOrder.h"
#include "GTLib/GTLib.h"
class Actor;
using namespace std;

class QuadTree {
public:
	QuadTree(int id = 0);
	~QuadTree();
	void add_obj(QuadTreeObj<Actor>* add);//空間に追加
	void change_last(QuadTreeObj<Actor>* change);//末尾の変更
	void remove_obj();//空間から離脱
	void clear();//データ削除
	void set_child(int child);//子空間登録
	void set_pearent(int pearent) { pearent_ = pearent; }//親空間登録
	int space_id()const { return space_id_; }//空間番号
	QuadTreeObj<Actor>* last_object()const { return last_object_; }//末尾のオブジェクト
	QuadTreeObj<Actor>* first_object()const { return first_object_; }//先頭のオブジェクト
	void last_object(QuadTreeObj<Actor>* change){ last_object_= change; }//末尾のオブジェクト変更
	void first_object(QuadTreeObj<Actor>* change) { first_object_= change; }//先頭のオブジェクト変更
	int object_count()const { return object_count_; }//この空間に属するオブジェクトの数
	int child_id(int at)const { return (at>=4||at<0)?-1:children_[at]; }//子空間のID
	int child_count()const { return current_child_count_; }//子空間の数
private:
	int children_[4]{0,0,0,0};//子空間
	int current_child_count_{ 0 };//子空間の保存数
	int pearent_{0};//親空間
	QuadTreeObj<Actor>* last_object_{nullptr};//末尾
	QuadTreeObj<Actor>* first_object_{ nullptr };//先頭
	int object_count_{ 0 };//この空間にあるオブジェクトの数
	int space_id_{ 0 };//空間の番号
};

#endif