//#ifndef EDITOR_H_
//#define EDITOR_H_
//
//#include"IEditor.h"
//
////ながそんをベースとしたデータを「読み込み、編集、書き出し」をするためのクラス
//class Editor {
//public:
//	Editor(string name, string key, Rows rule, Rows data, vector<DevideRow> new_rule);
//	void update(float time=0);
//	void clear(){}
//	Rows result()const { return result_; }
//	string key()const { return key_; }
//	bool is_closed()const { return is_closed_; }
//	void output_result();//書き出し用データに変換
//private:
//	Row cut_data(string data);
//	int judge_data(const int data_count, RowsManager* data, Row rule);//一行を分解しウィンドウに変換
//	int add_contents(int row_num, const int rule_row_num, Row rule, const bool is_new_rule);//ルールをもとにウィンドウに要素を追加
//	void input_ini_data(const int row_num, string id, string data, const bool is_add_id);//指定したデータを保存
//	bool baseic_data(const int row_num, int& j, string id, const bool is_add_id);//基本的なデータを保存
//	bool make_window(const int data_count, Row data_list, Row rule, string id, int& i, int& j, char ui_id[]);//データをもとに編集用ウィンドウを生成
//	void creat_ini_data(const int row_count);
//private:
//	string name_ = "";//ウィンドウ名
//	RowsManager data_;
//	RowsManager rule_;
//	int element_add_id_{ 0 };//要素追加する行
//	int add_function_id_{ 0 };//機能追加に使うやつ
//	int new_rule_count_{ 0 };
//	bool is_ini_{ false };//初期化フラグ
//	vector<int> before_function_id_;//ファンクションＩＤの変更をチェックするやつ
//	vector<DevideRow> new_rule_;
//	string key_ = "";
//	Rows result_;//最終的にテキストファイルに保存するやつ
//	bool is_closed_{ false };
//};
//
//#endif