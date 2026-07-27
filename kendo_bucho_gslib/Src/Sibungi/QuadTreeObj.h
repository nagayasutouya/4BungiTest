#ifndef QUADTREE_OBJ_H_
#define QUADTREE_OBJ_H_

class QuadTreeSpace;

template<class T>
class QuadTreeObj {
public:
	QuadTreeObj(){}
	~QuadTreeObj() {
		clear();
	}
	bool remove() {//離脱
		//離脱済みはキャンセル//空間離脱通知に失敗したらキャンセル
		if (space_ == nullptr|| !space_->removed(this))return false;
		//前後をくっつける
		docking_p_and_n();
		p_obj_ = n_obj_ = nullptr;
		space_ = nullptr;
		return true;
	}
	void clear() {
		space_ = nullptr;
	    obj_ = nullptr;
		docking_p_and_n();
		p_obj_ = n_obj_ = nullptr;
	}
	QuadTreeSpace* space()const { return space_; }//空間
	void space(QuadTreeSpace* space) { space_=space; }//空間
	T* obj()const { return obj_; }//本体
	void obj(T* obj) { obj_=obj; }//本体
	QuadTreeObj* p_obj()const { return p_obj_; };//前
	void p_obj(QuadTreeObj* p_obj) {  p_obj_=p_obj; };//前
	QuadTreeObj* n_obj()const { return n_obj_; }//次
	void n_obj(QuadTreeObj* n_obj) { n_obj_=n_obj; }//次
private:
	void docking_p_and_n() {//前後をくっつける
		if (p_obj_ != nullptr)p_obj_->n_obj(n_obj_);
		if (n_obj_ != nullptr)n_obj_->p_obj(p_obj_);
	}
private:
	QuadTreeSpace* space_ = nullptr;//空間
	T* obj_ = nullptr;//本体
	QuadTreeObj* p_obj_ = nullptr;//前
	QuadTreeObj* n_obj_ = nullptr;//次
};

#endif