#include "QuadTreeManager.h"
#include "Actor.h"
static const int SpaceCount{ 85 };
QuadTreeManager::QuadTreeManager() {
	for (int i = 0; i < SpaceCount; i++) {
		//空間を生成
		space_.push_back(make_unique<QuadTree>(i));
		//親空間に接続
		int pearent_id=(i-1)/4;
		if (pearent_id >= 0 && i != 0) {
			space_.at(i)->set_pearent(pearent_id);
			space_.at(pearent_id)->set_child(i);
		}
	}
}
//別空間へ移動する
void QuadTreeManager::change_space(QuadTreeObj<Actor>* target,int space_id){
	QuadTree* before_space = target->space;
	if (before_space != nullptr) {
		if (before_space->space_id() == space_id)return;
		//自分の前後を接続
		if (target->p_obj != nullptr)target->p_obj->n_obj = target->n_obj;
		else before_space->first_object(target->n_obj);//先頭を変更
		if (target->n_obj != nullptr)target->n_obj->p_obj = target->p_obj;
		//空間に離脱を宣言
		before_space->remove_obj();
		//自身が末尾なら一個前を末尾に
		if (target->n_obj == nullptr)before_space->change_last(target->p_obj);
		//接続を解除
		target->p_obj = target->n_obj = nullptr;
	}
	//次の空間がないなら処理終わり
	if (target->obj->is_dead() || space_id< 0)return;
	//空間を変更
	QuadTree* space = find_space(space_id);
	target->space=space;
	//変更後空間に追加を宣言
	space->add_obj(target);
}
//空間が存在するかチェック
QuadTree* QuadTreeManager::find_space(int space_id){
	//範囲外ならnull
	if (space_id >= SpaceCount||space_id<0)return nullptr;
	return space_[space_id].get();
}
//モートン序列の更新と当たり判定のチェック
void QuadTreeManager::update() {
	//チェック済み空間かどうか
	vector<bool> check;
	for (int i = 0; i < SpaceCount; i++)check.push_back(false);
	//チェック済みの数
	int checked_space_count = 0;
	int space_id = 0;
	while (checked_space_count < SpaceCount) {
		//今からチェックするところ
		QuadTree* current = space_[space_id].get();
		one_space_loop(space_id,checked_space_count,check,nullptr);
	}
}
//一空間の処理
void QuadTreeManager::one_space_loop(int id,int& checked_space_count,vector<bool>& check, QuadTreeObj<Actor>* parent_col) {
	QuadTree* current = space_[id].get();
	QuadTreeObj<Actor>* col = current->last_object();
	//チェック済みなら無視
	if (!check[id]) {
		checked_space_count++;
		check[id] = true;
		//親空間のオブジェ群と接続
		if (parent_col != nullptr&& current->first_object()!=nullptr) {
			parent_col->n_obj = current->first_object();
			current->first_object()->p_obj = parent_col;
		}
		collide(col);
	}
	//子があるならそっちへ移動
	for (int i = 0; i < current->child_count(); i++) {
		one_space_loop(current->child_id(i),checked_space_count,check,col);
	}
	//別空間との連結を削除
	if (parent_col != nullptr) {
		if(parent_col->n_obj!=nullptr)parent_col->n_obj->p_obj = nullptr;
		parent_col->n_obj = nullptr;
	}
}
//当たり判定を取る
void QuadTreeManager::collide(QuadTreeObj<Actor>* col){
	//オブジェクトがあれば当たり判定を取る
	if (col != nullptr) {
		QuadTreeObj<Actor>* p_col = col->p_obj;
		while (col != nullptr) {
			//先頭につくまで
			while (p_col != nullptr) {
				//当たり判定チェック
				col->obj->collide(*p_col->obj);
				p_col = p_col->p_obj;
			}
			//次の末尾チェック
			col = col->p_obj;
			if (col != nullptr)p_col = col->p_obj;
		}
	}
}
//データ削除
void QuadTreeManager::clear() {}