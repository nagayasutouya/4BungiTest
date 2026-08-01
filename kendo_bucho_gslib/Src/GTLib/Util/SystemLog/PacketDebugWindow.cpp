//#include"PacketDebugWindow.h"
//#include"imgui/imgui.h"
//
////•`‰æİ’è
//void PacketDebugWindow::ini_propaty(int draw_coount){
//#ifdef _DEBUG
//	draw_max_ = draw_coount;
//	//•`‰æ”‚É§ŒÀ‚ğ‚Â‚¯‚é‚©‚Ç‚¤‚©
//	is_delete_ = (draw_max_ < 0)? false:true;
//#endif //_DEBUG
//}
////ƒƒO’Ç‰Á
//void PacketDebugWindow::add_log(string data){
//#ifdef _DEBUG
//	log_.push_back(data);
//#endif //_DEBUG
//}
////•`‰æ
//void PacketDebugWindow::update_window(string window_name){
//#ifdef _DEBUG
//	ImGui::Begin(window_name.c_str());
//	for (unsigned int i = 0; i < log_.size(); i++) {
//		ImGui::Text(log_.at(i).c_str());
//	}
//	ImGui::End();
//	//Å‘å”’´‚¦‚½‚çÁ‚·
//	if (is_delete_&&log_.size()>=(unsigned)draw_max_) {
//		log_.pop_front();
//	}
//#endif //_DEBUG
//}