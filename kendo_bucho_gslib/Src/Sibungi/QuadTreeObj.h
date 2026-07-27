#ifndef QUAD_TREE_OBJ_H_
#define QUAD_TREE_OBJ_H_

class QuadTreeSpace;

template<class T>
struct QuadTreeObj {
	QuadTreeSpace* space = nullptr;//空間
	T* obj = nullptr;//本体
	QuadTreeObj* p_obj = nullptr;//前
	QuadTreeObj* n_obj = nullptr;//次
	void clear() {
		space = nullptr;
		obj = nullptr;
		//前後をくっつける
		docking_p_and_n();
		p_obj = n_obj = nullptr;
	}
	bool remove() {//離脱
		//離脱済みはキャンセル//空間離脱通知に失敗したらキャンセル
		if (space == nullptr || !space->removed(this))return false;
		//前後をくっつける
		docking_p_and_n();
		p_obj = n_obj = nullptr;
		space = nullptr;
		return true;
	}
	void docking_p_and_n() {//前後をくっつける
		if (p_obj != nullptr)p_obj->n_obj=n_obj;
		if (n_obj != nullptr)n_obj->p_obj=p_obj;
	}
};

#endif