//#include"IEditorManager.h"
//
//void IEditorManager::update(float time) {}
//void IEditorManager::clear() {}
//void IEditorManager::draw()const {}
//
//void IEditorManager::base_update(float time){
//	for (const auto& editor : editors_) {
//		editor->update(time);
//	}
//}
//
//void IEditorManager::base_clear(){
//	for (const auto& editor : editors_) {
//		editor->clear();
//	}
//}
//
//void IEditorManager::base_draw()const{
//	for (const auto& editor : editors_) {
//		editor->draw();
//	}
//}
//
//void IEditorManager::handle_message(const std::string& message, void* param) {
//	for (const auto& editor : editors_) {
//		editor->handle_message(message,param);
//	}
//}
//
//void IEditorManager::add_editor(IEditor editor){
//	editors_.push_back(std::make_unique<IEditor>(editor));
//}