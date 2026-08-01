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
<<<<<<< HEAD
		if (space_ == nullptr|| !space_->removed(this))return false;
=======
		if (space_ == nullptr|| !space_->removed(this,false))return false;
>>>>>>> Ver2.0
		//前後をくっつける
		docking_p_and_n();
		p_obj_ = n_obj_ = nullptr;
		space_ = nullptr;
		return true;
	}
	void clear() {
<<<<<<< HEAD
=======
		if(space_!=nullptr)space_->removed(this, true);
>>>>>>> Ver2.0
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
<<<<<<< HEAD
	void p_obj(QuadTreeObj* p_obj) {  p_obj_=p_obj; };//前
	QuadTreeObj* n_obj()const { return n_obj_; }//次
	void n_obj(QuadTreeObj* n_obj) { n_obj_=n_obj; }//次
=======
	void p_obj(QuadTreeObj* p_obj) {  if(p_obj!=this)p_obj_=p_obj; };//前
	QuadTreeObj* n_obj()const { return n_obj_; }//次
	void n_obj(QuadTreeObj* n_obj) { if (n_obj != this)n_obj_=n_obj; }//次
	void delete_obj() { is_delete_ = true; }//削除
	bool is_delete()const { return is_delete_; }//削除するかどうか
>>>>>>> Ver2.0
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
<<<<<<< HEAD
=======
	bool is_delete_{ false };//削除フラグ
>>>>>>> Ver2.0
};

#endif