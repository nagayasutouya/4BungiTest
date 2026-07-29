#ifndef QUAD_TREE_OBJ_H_
#define QUAD_TREE_OBJ_H_

class QuadTreeSpace;

template<class T>
class QuadTreeObj {
public:
	void clear() {
		space_ = nullptr;
		obj_ = nullptr;
		//前後をくっつける
		docking_p_and_n();
		p_obj_ = n_obj_ = nullptr;
	}
	bool remove() {//離脱
		//離脱済みはキャンセル//空間離脱通知に失敗したらキャンセル
		if (space_ == nullptr || !space_->removed(this))return false;
		//前後をくっつける
		docking_p_and_n();
		p_obj_ = n_obj_ = nullptr;
		space_ = nullptr;
		return true;
	}
	QuadTreeSpace* space()const { return space_; }//所属空間
	T* obj()const { return obj_; }//本体
	QuadTreeObj* p_obj()const { return p_obj_ ; };//前
	QuadTreeObj* n_obj()const { return n_obj_ ; };//次
	void space(QuadTreeSpace* change){ space_=change; }//所属空間
	void obj(T* change) { obj_ = change; };//本体設定
	void p_obj(QuadTreeObj* change) { if (change != this)p_obj_ = change; printf("変更指示\n"); };//前変更
	void n_obj(QuadTreeObj* change) { if (change != this)n_obj_ = change; printf("変更指示\n"); };//次変更
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