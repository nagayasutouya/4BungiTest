#ifndef QUADTREE_MANAGER_H_
#define QUADTREE_MANAGER_H_

#include "QuadTreeSpace.h"
class QuadTreeSpace;
class Actor;
using namespace std;
class QuadTreeManager {
public:
	~QuadTreeManager() { clear(); };
	QuadTreeObj<Actor>* add_actor(Actor* actor);//アクター追加
	void entry_object_to_space(Vector3 pos, float radius, QuadTreeObj<Actor>* target);//オブジェクトを空間に入れる
	void collide();//当たり判定
	void clear();//消すやつ　アクマネでやる
private:
	void creat_space(GTDWORD id);//空間作成
	void one_space_collide(int space_id,QuadTreeObj<Actor>*parent_last_obj);//一空間のループ
private:
	const GTUINT SpaceNumMax{ 85 };
	QuadTreeSpace* space_array_[85];
};

#endif