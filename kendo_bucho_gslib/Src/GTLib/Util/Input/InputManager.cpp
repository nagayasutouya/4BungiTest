//#include"InputManager.h"
//#include"gslib.h"
//float InputManager::GetVirtical(){
//	return (controller_.is_connected()) ? controller_.get_left_stick_y(dead_zone_[1]) : (gsGetKey() == GKEY_W) ? gsGetKeyState(GKEY_W) : -gsGetKeyState(GKEY_S);
//}
//float InputManager::GetHorizontal(){
//	return (controller_.is_connected()) ? controller_.get_left_stick_x(dead_zone_[0]) : (gsGetKey() == GKEY_A) ? -gsGetKeyState(GKEY_A) : gsGetKeyState(GKEY_D);
//}
//float InputManager::GetVirticalDown() {
//	return (controller_.is_connected()) ? controller_.get_left_stick_y_down(dead_zone_[1]) : (gsGetKey() == GKEY_W) ? gsGetKeyTrigger(GKEY_W) : -gsGetKeyTrigger(GKEY_S);
//}
//float InputManager::GetHorizontalDown() {
//	return (controller_.is_connected()) ? controller_.get_left_stick_x_down(dead_zone_[0]) : (gsGetKey() == GKEY_A) ? -gsGetKeyTrigger(GKEY_A) : gsGetKeyTrigger(GKEY_D);
//}
//float InputManager::GetRVirtical() {
//	return (controller_.is_connected()) ? controller_.get_right_stick_y(dead_zone_[1]) : (gsGetKey() == GKEY_DOWNARROW) ? -gsGetKeyState(GKEY_DOWNARROW) : gsGetKeyState(GKEY_UPARROW);
//}
//float InputManager::GetRHorizontal() {
//	return (controller_.is_connected()) ? controller_.get_right_stick_x(dead_zone_[0]) : (gsGetKey() == GKEY_LEFTARROW) ? -gsGetKeyState(GKEY_LEFTARROW) : gsGetKeyState(GKEY_RIGHTARROW);
//}
//bool InputManager::Select(){
//	return /*(controller_.is_connected()) ? controller_.is_trigger_down(GS_XBOX_PAD_A) : gsGetKeyTrigger(GKEY_RETURN)*/controller_.is_trigger_down(GS_XBOX_PAD_A) || gsGetKeyTrigger(GKEY_RETURN);
//}
//bool InputManager::Back(){
//	return (controller_.is_connected()) ? controller_.is_trigger_down(GS_XBOX_PAD_BACK) : gsGetKeyTrigger(GKEY_BACK);
//}
//bool InputManager::Cancel() {
//	return (controller_.is_connected()) ? controller_.is_trigger_down(GS_XBOX_PAD_B) : gsGetKeyTrigger(GKEY_C);
//}
//bool InputManager::Option(){
//	return (controller_.is_connected()) ? controller_.is_trigger_down(GS_XBOX_PAD_START) : gsGetKeyTrigger(GKEY_TAB);
//}
//bool InputManager::GameEnd() {
//	return(controller_.is_pressed(GS_XBOX_PAD_START)&& controller_.is_pressed(GS_XBOX_PAD_BACK))|| gsGetKeyState(GKEY_ESCAPE);
//}
//short InputManager::GetInput(){
//	return(controller_.is_connected()) ? controller_.current_bottuns_state():gsGetKey();
//}
//bool InputManager::Reset() {
//	return(controller_.is_connected()) ? controller_.is_trigger_down(GS_XBOX_PAD_X) : gsGetKeyTrigger(GKEY_V);
//}
//bool InputManager::ADS() {
//	return(controller_.is_connected()) ? controller_.is_trigger_down(GS_XBOX_PAD_LEFT_SHOULDER) : gsGetKeyTrigger(GKEY_C);
//}
//bool InputManager::TutoReset() {
//	return gsGetKeyState(GKEY_SPACE);
//}
//void InputManager::set_dead_zone(float vir, float hor) {
//	dead_zone_[0] = hor;
//	dead_zone_[1] = vir;
//}