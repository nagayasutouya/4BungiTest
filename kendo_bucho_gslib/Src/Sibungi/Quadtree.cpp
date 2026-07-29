#include "QuadTree.h"
#include "Actor.h"
QuadTreeSpace::QuadTreeSpace(int id) :space_id_{ id }{}
QuadTreeSpace::~QuadTreeSpace(){
	clear();
}
//空間に追加
void QuadTreeSpace::add_obj(QuadTreeObj<Actor>* add){
	printf("Space:%d\n", space_id_);
	//すでにこの空間にいるか空っぽならばいばい
	if (add == nullptr || add->space() == this || add->obj() == nullptr) {
		printf("除外:%s\n",(add==nullptr|| add->obj() == nullptr)?"NULL":add->obj()->name().c_str());
		return;
	}
	//空なら
	if (last_object_ == nullptr && first_object_ == nullptr) {
		//先頭に登録
		first_object_ = last_object_ = add;
	}
	else if (last_object_ != nullptr&&last_object_!=add) {
		//末尾のオブジェクトに接続
		last_object_->n_obj(add);
		add->p_obj(last_object_);
		last_object_ = add;
	}
	//前空間から離脱
	add->remove();
	//空間を保存
	add->space(this);
}
//離脱通知の受け取り
bool QuadTreeSpace::removed(QuadTreeObj<Actor>* target) {
	printf("%d:離脱:%s\n",space_id_,target->obj()->name().c_str());
	//この空間に自分しかいないなら両方リセット
	if (first_object_ == target && last_object_ == target) {
		printf("誰もいない\n");
		first_object_ = last_object_ = nullptr;
	}
	//離脱するやつが先頭だったら次を先頭に
	else if (first_object_ == target) {
		if (target->n_obj() == nullptr || target->n_obj()->space() == this) {
			printf("先頭チェンジ\n"); 
			first_object_ = first_object_->n_obj();
		}
	}
	//離脱が末尾だったら一個前を末尾に
	else if (last_object_ == target) {
		if (target->p_obj() == nullptr || target->p_obj()->space() == this) {
			printf("末尾チェンジ\n");
			last_object_ = target->p_obj();
		}
	}
	return true;
}
//データ削除
void QuadTreeSpace::clear(){
	first_object_ =last_object_= nullptr;
}