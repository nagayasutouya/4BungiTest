#ifndef QUADTREE_MANAGER_H_
#define QUADTREE_MANAGER_H_

#include "QuadTreeSpace.h"
#include <vector>
#include <list>
<<<<<<< HEAD
class  CQuadTree;
class Actor;

class QuadTreeManager {
public:
	QuadTreeManager();
	~QuadTreeManager() { clear(); }
	void initialize(GTUINT  layer_num, Rect field_size);//初期化
	void add_object(Rect object_pos, QuadTreeObj<Actor>* target);//オブジェクトを空間に登録
	GTUINT make_collide_list(list<Actor*> actors);//チェックリストの作成
	vector<Actor*>collide_list()const { return collide_list_; }//チェックリスト
private:
	void creat_space(GTDWORD id);//新しく空間をつくる
	void one_space_collide(GTDWORD id,vector<Actor*>&collide_list,list<Actor*>&parents);//1空間のチェックリストを作る
	void clear();//データ削除
private:
	const GTUINT SpaceCount{ 85 };
	QuadTreeSpace* space_array_[85];
	Vector2 field_origin_point_{ 0.0f,0.0f };//空間の初期地点
	Vector2 field_size_{0.0f,0.0f};//空間の縦横
	Vector2 space_size_{ 0.0f,0.0f };//小空間1つの縦横
	GTUINT layer_num_max_{ 0 };//分割数
	vector<int> space_in_layer_;//1レイヤーにどれだけのスペースがはいるか
	GTUINT space_num_max_{ 0 };//全空間のトータル数
	vector<Actor*>collide_list_;//当たり判定チェックリスト
=======
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
>>>>>>> Ver2.0
};

#endif