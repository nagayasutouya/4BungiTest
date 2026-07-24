#ifndef QUADTREE_MANAGER_H_
#define QUADTREE_MANAGER_H_

#include "QuadTree.h"
#include <vector>
#include <list>

class QuadTreeManager {
public:
	QuadTreeManager();
	~QuadTreeManager() { clear(); }
	void change_space(QuadTreeObj<Actor>* target, int space_id=-1);//別空間へ移動する
	QuadTree* find_space(int space_id = -1);//空間が存在するかチェック
	void update();//当たり判定のチェック
private:
	void one_space_loop(int id, int& checked_space_count, vector<bool>& check, QuadTreeObj<Actor>* parent_col);//一空間の処理
	void collide(QuadTreeObj<Actor>* col);//当たり判定を取る
	void clear();//データ削除
private:
	vector<unique_ptr<QuadTree>> space_;//空間管理
};

#endif