#include"QuadTreeSpace.h"

QuadTreeSpace::QuadTreeSpace(int id) :space_id_{ id }{}
QuadTreeSpace::~QuadTreeSpace(){
	clear();
}
//空間に追加
bool QuadTreeSpace::add_obj(QuadTreeObj<Actor>* add){
	//すでにこの空間にいるか空っぽならばいばい
	if (add->space() == this || add->obj() == nullptr)return false;
	if (first_object_ != nullptr) {
		//先頭のオブジェクトに接続
		first_object_->p_obj(add);
		add->n_obj(first_object_);
		first_object_ = add;
	}
	else {
		//先頭に登録
		first_object_ = add;
	}
	//空間を保存
	add->space(this);
	return true;
}
//データ削除
void QuadTreeSpace::clear(){
	space_id_ = 0;
}
//離脱通知の受け取り
bool QuadTreeSpace::removed(QuadTreeObj<Actor>*target){
	//離脱するやつが先頭だったら変更
	if (first_object_ == target) {
		first_object_ = first_object_->n_obj();
	}
	return true;
}