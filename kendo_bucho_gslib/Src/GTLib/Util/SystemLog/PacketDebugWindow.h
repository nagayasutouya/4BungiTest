//#ifndef PACKET_DEBUG_WINDOW_H_
//#define PACKET_DEBUG_WINDOW_H_
//
//#include<deque>
//#include<string>
//using namespace std;
//
////ログをインゲームのウィンドウに出すクラス
//class PacketDebugWindow {
//public:
//	PacketDebugWindow() = default;
//	~PacketDebugWindow() { log_.clear(); }
//	void ini_propaty(int draw_coount);//描画設定 0未満なら無限描画
//	void add_log(string data);//ログ追加
//	void update_window(string window_name);//描画
//private:
//	deque<string> log_;//データ描画
//	bool is_delete_{ false };//ログを消すか
//	int draw_max_{ 0 };//描画数の最大
//};
//
//#endif