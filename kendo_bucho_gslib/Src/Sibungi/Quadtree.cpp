#include"QuadTree.h"

QuadTree::QuadTree(int id) :space_id_{ id }{
}
QuadTree::~QuadTree(){
	clear();
}
//空間に追加
void QuadTree::add_obj(QuadTreeObj<Actor>* add){
	object_count_++;
	if (last_object_ != nullptr) {
		//末尾のオブジェクトに接続
		last_object_->n_obj = add;
		add->p_obj = last_object_;
	}
	else {
		//先頭に登録
		first_object_ = last_object_ = add;
	}
	//自分が末尾になる
	change_last(add);
}
//末尾変更
void QuadTree::change_last(QuadTreeObj<Actor>* change) {
	last_object_ = change;
}
//空間から離脱
void QuadTree::remove_obj() { 
	object_count_--;
	//だれもいなくなった
	if (object_count_ <= 0) {
		first_object_ = nullptr;
		last_object_ = nullptr;
		object_count_ = 0;
	}
}
//データ削除
void QuadTree::clear(){
	last_object_ = nullptr;
	object_count_ = 0;
}
//子空間登録
void QuadTree::set_child(int child){
	if (current_child_count_ >= 4)return;
	children_[current_child_count_] = child;
	current_child_count_++;
}