#include "QuadTreeManager.h"
#include "Actor.h"
static const int SpaceCount{ 85 };
QuadTreeManager::QuadTreeManager() {}
void QuadTreeManager::initialize(Rect field_size) {//初期化
	space_num_max_ = 85;
	MotornOrder::instance()->set_space_size(field_size);
	log_.set_propaty("LocalLog/" + log_.current_day() + '/' + "QuadTreeManager" + '/' + log_.creat_file_name("QuadTreeManager"), true,-1);
}
//別空間へ移動する
void QuadTreeManager::change_space(Rect object_pos,QuadTreeObj<Actor>* target){
	//場所出す
	int space = MotornOrder::instance()->morton_order(object_pos);
	//指定空間内なら登録
	if (space >= 0 && space <= space_num_max_) {
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
void QuadTreeManager::collide() {
	//空間ないなら無視
	if (space_array_[0] == nullptr)return;
	one_space_loop(0, nullptr,check_start_);
	check_start_++;
}
//一空間の処理
void QuadTreeManager::one_space_loop(int id, QuadTreeObj<Actor>* parent_end, int check) {
	char buf[512];
	snprintf(buf,sizeof(buf),"%d:Space%d",check,id);
	log_.creat_log(buf);
	//今チェックする空間
	QuadTreeSpace* current= space_array_[id];
	//先頭を取得
	QuadTreeObj<Actor>* col = current->first_object();
	//親空間のオブジェ群と接続
	if (parent_end != nullptr && col != nullptr) {
		snprintf(buf, sizeof(buf), "Docking");
		log_.creat_log(buf);
		//別のと接続してたら切る
		disconnect_next_obj(parent_end);
		parent_end->n_obj = col;
		col->p_obj = parent_end;
	}
	//現在地を末尾に変更
	col = current->last_object();
	//オブジェクトがあれば当たり判定を取る
	while (col != nullptr) {
		QuadTreeObj<Actor>* p_col = col->p_obj;
		//先頭につくまで
		while (p_col != nullptr) {
			snprintf(buf, sizeof(buf), "Chack:%s,%s", col->obj->name().c_str(), p_col->obj->name().c_str());
			log_.creat_log(buf);
			//衝突判定
			col->obj->collide(*p_col->obj);
			//次のへ
			p_col = p_col->p_obj;
		}
		//次のチェック
		col = col->p_obj;
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
			//子空間に移動
			one_space_loop(next_space,current->last_object(),check);
		}
	}
	//親空間との連結を削除
	if (parent_end != nullptr)disconnect_next_obj(parent_end);
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
//対象オブジェの末尾接続をきる
void QuadTreeManager::disconnect_next_obj(QuadTreeObj<Actor>* target){
	if (target->n_obj != nullptr)target->n_obj->p_obj = nullptr;
	target->n_obj = nullptr;
}