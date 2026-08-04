#include"QuadTreeSpace.h"
#include"Actor.h"
QuadTreeSpace::QuadTreeSpace(int id) :space_id_{ id }{}
QuadTreeSpace::~QuadTreeSpace(){
	clear();
}
//空間に追加
bool QuadTreeSpace::add_obj(QuadTreeObj<Actor>* add){
	//すでにこの空間にいるか空っぽならばいばい
	if (add->space() == this || add->obj() == nullptr)return false;
	//printf("空間に加入:%d:%s\n",space_id_,add->obj()->name().c_str());
	//前空間から離脱
	add->remove();
	if (last_object_ != nullptr) {
		//先頭のオブジェクトに接続
		last_object_->n_obj(add);
		add->p_obj(last_object_);
		last_object_ = add;
		//printf("末尾に加入\n");
	}
	else {
		//先頭に登録
		first_object_= last_object_ = add;
		//printf("先頭に加入\n");
	}
	//空間を保存
	add->space(this);
	return true;
}
//データ削除
void QuadTreeSpace::clear(){
	space_id_ = 0;
	first_object_ = nullptr;
}
//離脱通知の受け取り
bool QuadTreeSpace::removed(QuadTreeObj<Actor>*target,bool forced){
	//別空間か無効なら無視
	if (target == nullptr || (!forced&&target->space() != this)) {
		//printf("離脱キャンセル:%d:%s\n", space_id_, (target == nullptr) ? "NULL" : target->obj()->name().c_str());
		return false;
	}
	//離脱するやつが末尾だったら変更
	if (last_object_==target) {
		//printf("末尾変更:%d:%s\n", space_id_, (last_object_ == nullptr) ? "NULL" : last_object_->obj()->name().c_str());
		last_object_ = last_object_->p_obj();
		//printf("末尾変更後:%d:%s\n", space_id_, (last_object_ == nullptr) ? "NULL" : last_object_->obj()->name().c_str());
	}
	//離脱するやつが先頭だったら変更
	if (first_object_ == target) {
		//printf("先頭変更:%d:%s\n", space_id_, (first_object_ == nullptr) ? "NULL" : first_object_->obj()->name().c_str());
		first_object_ = first_object_->n_obj();
		//printf("先頭変更後:%d:%s\n", space_id_, (first_object_ == nullptr) ? "NULL" : first_object_->obj()->name().c_str());
	}
	return true;
}