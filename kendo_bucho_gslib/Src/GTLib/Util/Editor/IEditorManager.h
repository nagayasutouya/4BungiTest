//#ifndef IEDITOR_MANAGER_H_
//#define IEDITOR_MANAGER_H_
//
//#include<string>
//#include<list>
//#include<memory>
//#include"IEditor.h"
//
//class IEditorManager {
//public:
//	void base_update(float time = 0.0f);
//	void base_clear();
//	void base_draw()const;
//	virtual void update(float time = 0.0f);
//	virtual void clear();
//	virtual void draw()const;
//	void handle_message(const std::string& message, void* param);//メッセージ処理
//	void add_editor(IEditor editor);
//protected:
//	std::list<std::unique_ptr<IEditor>> editors_;
//};
//
//#endif