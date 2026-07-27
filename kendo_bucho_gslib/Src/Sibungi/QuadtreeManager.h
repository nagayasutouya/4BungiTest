#ifndef QUADTREE_MANAGER_H_
#define QUADTREE_MANAGER_H_

#include "QuadTree.h"
#include <vector>
#include <list>
class Actor;

using namespace std;

class QuadTreeManager {
public:
	QuadTreeManager();
	~QuadTreeManager() { clear(); }
	void initialize(Rect field_size);//初期化
	void change_space(Rect object_pos, QuadTreeObj<Actor>* target);//別空間へ移動する
	void creat_space(GTDWORD id);//空間が存在するかチェック
	GTUINT creat_collide_list(list<Actor*> actors);//当たり判定のチェック
	vector<Actor*>collide_list()const { return collide_list_; }
private:
	void one_space_loop(int id, vector<Actor*>& collide_list, list<Actor*>& parents);//一空間の処理
	void clear();//データ削除
private:
	const int layer_num_max_{ 5 };//最大レイヤー数
	int space_num_max_{ 0 };//空間最大数
	QuadTreeSpace* space_array_[85];//空間管理
	vector<Actor*>collide_list_;//当たり判定チェックリスト
};

#endif