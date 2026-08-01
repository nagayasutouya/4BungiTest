#include "QuadTreeManager.h"
#include "Actor.h"
<<<<<<< HEAD
QuadTreeManager::QuadTreeManager() {}
void QuadTreeManager::initialize(GTUINT  layer_num, Rect field_size) {//初期化
	//各レイヤーに何個の空間があるか出す
	space_in_layer_.push_back(1);//最初は一個
	for (GTUINT i = 1; i < layer_num; i++)space_in_layer_.push_back(space_in_layer_[i - 1] * 4);//４つずつ増やしてく
	space_num_max_ = (space_in_layer_[layer_num - 1] - 1) / 3;
	//分割の最大数を超えれないように
	if (space_num_max_ > SpaceCount)return;
	//配列の先頭保存
	//space_array_ = new QuadTreeSpace();
	//ZeroMemory(space_array_, sizeof(QuadTreeSpace) * space_num_max_);
	//空間の大きさを登録
	field_origin_point_ = Vector2{ field_size.left ,field_size.top };
	field_size_ = Vector2{ field_size.right - field_size.left,field_size.top - field_size.bottom };
	MotornOrder::instance()->set_space_size(field_size);
	//分割数をもとに最小空間のサイズを出す (1<<layer_num)はレイヤー分bitをずらすことにより乗計算をするやつ
	space_size_ = Vector2{ field_size_.x / (1 << layer_num),field_size_.y / (1 << layer_num) };
	layer_num_max_ = layer_num;
}
//オブジェクトを空間に登録
void QuadTreeManager::add_object(Rect object_pos, QuadTreeObj<Actor>* target){
	//場所出す
	int space = MotornOrder::instance()->morton_order(object_pos);
	//指定空間内なら登録
	if (space>=0&&(unsigned)space <= space_num_max_) {
		creat_space((GTDWORD)space);
		space_array_[space]->add_obj(target);
	}
}
//空間の作成
void QuadTreeManager::creat_space(GTDWORD id) {
	while(!space_array_[id]){
		//空間作成
		space_array_[id] = new QuadTreeSpace(id);
		id = (id - 1) >> 2;
		//空間の最大値を超えたらおしまい
		if (id >= space_num_max_)break;
	}
}
//衝突判定チェック
GTUINT QuadTreeManager::make_collide_list(list<Actor*> actors) {
	collide_list_.clear();
	//誰もいない
	if (space_array_[0] == nullptr)return 0;
	list<Actor*>ParentsObjects;
	one_space_collide(0, collide_list_, ParentsObjects);
	return (GTUINT)collide_list_.size();
}
//1空間のチェックリストを作る
void QuadTreeManager::one_space_collide(GTDWORD id, vector<Actor*>& collide_list,list<Actor*>&parents){
	list<Actor*>::iterator it;
	//先頭を取得
	QuadTreeObj<Actor>* obj_1 = space_array_[id]->first_object();
	//チェック対象がなくなるまで
	while (obj_1 != nullptr) {
		QuadTreeObj<Actor>* obj_2 = obj_1->n_obj();
		//コリジョンぶつけるやつらを保存
		while (obj_2 != nullptr) {
			collide_list.push_back(obj_1->obj());
			collide_list.push_back(obj_2->obj());
			obj_2 = obj_2->n_obj();//次のチェック
		}
		//親空間のオブジェをチェック
		for (it = parents.begin(); it != parents.end();it++) {
			collide_list_.push_back(obj_1->obj());
			collide_list_.push_back(*it);
		}
		obj_1 = obj_1->n_obj();
	}
	bool has_child = false;
=======
//アクター追加
QuadTreeObj<Actor>* QuadTreeManager::add_actor(Actor* actor) {
	QuadTreeObj<Actor>* actor_obj = new QuadTreeObj<Actor>{};
	actor_obj->obj(actor);
	return actor_obj;
}
//当たり判定
void QuadTreeManager::collide(){	
	//誰もいないなら無視
	if (space_array_[0] == nullptr)return;
	printf("チェック開始\n");
	//当たり判定チェック開始
	one_space_collide(0, nullptr);
}
//一空間のループ
void QuadTreeManager::one_space_collide(int space_id, QuadTreeObj<Actor>*parent_last_obj){

	//現在空間データの取得
	QuadTreeSpace* current_space = space_array_[space_id];
	if (current_space != nullptr) {
		//先頭を取得
		QuadTreeObj<Actor>*current_obj = current_space->first_object();
		printf("Space:%d First:%s\n",current_space->space_id(),(current_obj==nullptr)?"NULL" : current_obj->obj()->name().c_str());
		//上位レイヤーの空間の末尾オブジェがあったら
		if (current_obj!=nullptr&&parent_last_obj != nullptr) {
			//お互いの接続部分を一応初期化
			current_obj->p_obj()->n_obj(nullptr);
			parent_last_obj->n_obj()->p_obj(nullptr);
			//ドッキング
			current_obj->p_obj(parent_last_obj);
			parent_last_obj->n_obj(current_obj);
		}
		//末尾へ移動
		current_obj = current_space->last_object();
		printf("Space:%d Last:%s\n", current_space->space_id(), (current_obj == nullptr) ? "NULL" : current_obj->obj()->name().c_str());
		//先頭までサーチ
		while (current_obj != nullptr) {
			//一個前とチェック
			QuadTreeObj<Actor>* check_obj = current_obj->p_obj();
			//オブジェの本体が存在しているか
			if (current_obj->obj() != nullptr) {
				//先頭につくまでの間
				while (check_obj != nullptr) {
					//オブジェの本体が存在しているか
					if (check_obj->obj() != nullptr) {
						//当たり判定
						current_obj->obj()->collide(*check_obj->obj());
					}
					//一個前へ
					if(check_obj!=nullptr)check_obj = check_obj->p_obj();
				}
			}
			//１個前へ
			if(current_obj!=nullptr)current_obj = current_obj->p_obj();
		}
		//子空間に渡すように
		parent_last_obj = current_space->last_object();
	}
>>>>>>> Ver2.0
	//子空間のチェック
	GTDWORD obj_num = 0;
	GTDWORD next_space = 0;
	for (GTDWORD i = 0; i < 4; i++) {
		//子のID
<<<<<<< HEAD
		next_space = id * 4 + 1 + i;
		//空間があるかチェック
		if (next_space < (GTDWORD)space_num_max_ && space_array_[next_space]) {
			//1度だけ親オブジェ群を保存
			if (!has_child) {
				obj_1= space_array_[id]->first_object();
				while (obj_1 != nullptr) {
					parents.push_back(obj_1->obj());
					obj_num++;
					obj_1 = obj_1->n_obj();
				}
			}
			has_child = true;
			//子空間に移動
			one_space_collide(next_space,collide_list,parents);
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
=======
		next_space = space_id * 4 + 1 + i;
		//空間があるかチェック
		if (next_space < (GTDWORD)SpaceNumMax && space_array_[next_space]) {
			//子空間に移動
			one_space_collide(next_space,parent_last_obj);
		}
	}
	if (current_space != nullptr) {
		//子との接続を解除
		if (current_space->first_object() != nullptr) {
			current_space->first_object()->p_obj(nullptr);
			printf("親解除:%d Last:%s\n", current_space->space_id(), (current_space->first_object()->obj() == nullptr) ? "NULL" : current_space->first_object()->obj()->name().c_str());
		}
		if (current_space->last_object() != nullptr) {
			current_space->last_object()->n_obj(nullptr);
			printf("子解除:%d Last:%s\n", current_space->space_id(), (current_space->last_object()->obj() == nullptr) ? "NULL" : current_space->last_object()->obj()->name().c_str());
		}
	}
}
//空間の作成
void QuadTreeManager::creat_space(GTDWORD id) {
	while (!space_array_[id]) {
		printf("CreatSpace:%d\n",(int)id);
		//空間作成
		space_array_[id] = new QuadTreeSpace(id);
		id = (id - 1) >> 2;
		//空間の最大値を超えたらおしまい
		if (id >= SpaceNumMax)break;
	}
}
//オブジェクトを空間に登録
void QuadTreeManager::entry_object_to_space(Vector3 pos,float radius, QuadTreeObj<Actor>* target) {
	Rect object_pos{pos.x-radius,pos.z+radius,pos.x+radius,pos.z-radius};
	//場所出す
	int space = MotornOrder::instance()->morton_order(object_pos);
	//指定空間内なら登録
	if (space >= 0 && (unsigned)space <=SpaceNumMax) {
		creat_space((GTDWORD)space);
		space_array_[space]->add_obj(target);
	}
}

void  QuadTreeManager::clear() {
	for (int i = 0; i < SpaceNumMax; i++) {
		delete space_array_[i];
>>>>>>> Ver2.0
	}
}