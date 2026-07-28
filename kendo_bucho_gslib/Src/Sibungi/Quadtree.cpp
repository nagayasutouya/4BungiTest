#include"QuadTree.h"

QuadTreeSpace::QuadTreeSpace(int id) :space_id_{ id }{}
QuadTreeSpace::~QuadTreeSpace(){
	clear();
}
//空間に追加
void QuadTreeSpace::add_obj(QuadTreeObj<Actor>* add){
	//すでにこの空間にいるか空っぽならばいばい
	if (add->space == this || add->obj == nullptr)return;
	if (last_object_ != nullptr) {
		//末尾のオブジェクトに接続
		last_object_->n_obj=add;
		add->p_obj=last_object_;
		last_object_ = add;
	}
	else {
		//先頭に登録
		first_object_=last_object_ = add;
	}
	//前空間から離脱
	add->remove();
	//空間を保存
	add->space=this;
}
//離脱通知の受け取り
bool QuadTreeSpace::removed(QuadTreeObj<Actor>* target) {
	//離脱するやつが先頭だったら次を先頭に
	if (first_object_ == target)first_object_ = first_object_->n_obj;
	//離脱が末尾だったら一個前を末尾に
	if (last_object_ == target)last_object_ = target->p_obj;
	return true;
}
//データ削除
void QuadTreeSpace::clear(){
	first_object_ =last_object_= nullptr;
}