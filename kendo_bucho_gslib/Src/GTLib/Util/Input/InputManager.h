//#ifndef INPUT_MANAGER_H_
//#define INPUT_MANAGER_H_
//
//#include "KeyBoard.h"
//#include "GTLib/Util/ControllerXInput.h"
//
////次回は所有権みたいなのつくってもいいかも
//class InputManager {
//public:
//	static InputManager* Instance() {
//		static InputManager im;
//		return &im;
//	}
//	bool update() { return controller_.update(); }
//	float GetVirtical();//Lスティック上下　WS
//	float GetHorizontal();//Lスティック左右　AD
//	float GetVirticalDown();//Lスティック上下　WS
//	float GetHorizontalDown();//Lスティック左右　AD
//	float GetRVirtical();//Rスティック上下
//	float GetRHorizontal();//Rスティック左右　AD
//	bool Option();//オプション起動ボタン
//	bool Select();//決定ボタン
//	bool Back();//スタートボタン
//	bool Cancel();//キャンセルボタン
//	bool GameEnd();//ゲーム終了
//	bool Reset();//カメラリセット
//	bool ADS();//カメラのADS変更
//	bool TutoReset();//チュートリアルフラグのリセット
//	short GetInput();//なんか入力された
//	ControllerXInput controller() { return controller_; }
//	void set_dead_zone(float vir,float hor);
//private:
//	ControllerXInput controller_;
//	float dead_zone_[2]{0.1f,0.1f};
//};
//
//#endif