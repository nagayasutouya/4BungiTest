#include "QuadTreeManager.h"
#include "Actor.h"
static const int SpaceCount{ 85 };
QuadTreeManager::QuadTreeManager() {}
void QuadTreeManager::initialize(Rect field_size) {//初期化
	space_num_max_ = 85;
	MotornOrder::instance()->set_space_size(field_size);
}
//別空間へ移動する
void QuadTreeManager::change_space(Rect object_pos,QuadTreeObj<Actor>* target){
	//場所出す
	int space = MotornOrder::instance()->morton_order(object_pos);
	//指定空間内なら登録
	if (space >= 0 && (unsigned)space <= space_num_max_) {
		creat_space((GTDWORD)space);
		space_array_[space]->add_obj(target);
	}
}
//空間の作成
void QuadTreeManager::creat_space(GTDWORD id) {
	while (!space_array_[id]) {
		//空間作成
		space_array_[id] = new QuadTreeSpace(id);
		id = (id - 1) >> 2;
		//空間の最大値を超えたらおしまい
		if (id >= space_num_max_)break;
	}
}
//モートン序列の更新と当たり判定のチェック
GTUINT QuadTreeManager::creat_collide_list(list<Actor*> actors) {
	collide_list_.clear();
	//誰もいない
	if (space_array_[0] == nullptr)return 0;
	list<Actor*>ParentsObjects;
	one_space_loop(0,collide_list_,ParentsObjects);
	return (GTUINT)collide_list_.size();
}
//一空間の処理
void QuadTreeManager::one_space_loop(int id,vector<Actor*>& collide_list, list<Actor*>& parents) {
	//当たり判定チェックリスト追加
	list<Actor*>::iterator it;
	//先頭を取得
	QuadTreeObj<Actor>* col = space_array_[id]->first_object();
	//オブジェクトがあれば当たり判定を取る
	while (col != nullptr) {
		QuadTreeObj<Actor>* n_col = col->n_obj;
		//末尾につくまで
		while (n_col != nullptr) {
			//リストに追加
			collide_list.push_back(col->obj);
			collide_list.push_back(n_col->obj);
			//次のへ
			n_col = n_col->p_obj;
		}
		//次は親空間と
		for (it = parents.begin(); it != parents.end(); it++) {
			//リストに追加
			collide_list.push_back(col->obj);
			collide_list.push_back(*it);
		}
		//次のチェック
		col = col->n_obj;
	}
	//子があるならそっちへ移動
	bool has_child = false;
	//子空間のチェック
	GTDWORD obj_num = 0;
	GTDWORD next_space = 0;
	for (GTDWORD i = 0; i < 4; i++) {
		//子のID
		next_space = id * 4 + 1 + i;
		//空間があるかチェック
		if (next_space < (GTDWORD)space_num_max_ && space_array_[next_space]) {
			//1度だけ親オブジェ群を保存
			if (!has_child) {
				col = space_array_[id]->first_object();
				while (col != nullptr) {
					parents.push_back(col->obj);
					obj_num++;
					col = col->n_obj;
				}
			}
			has_child = true;
			//子空間に移動
			one_space_loop(next_space, collide_list, parents);
		}
	}
	//子の周回が終わったら親群を解散
	if (has_child) {
		for (GTDWORD i = 0; i < obj_num; i++) {
			parents.pop_back();
		}
	}
}
//データ削除
void QuadTreeManager::clear() {
	for (unsigned int i = 0; i < SpaceCount; i++) {
		if (space_array_[i] != nullptr) {
			delete space_array_[i];
			space_array_[i] = nullptr;
		}
	}
}