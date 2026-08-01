//#ifndef IEDITOR_H_
//#define IEDITOR_H_
//
//#include<vector>
//#include<string>
//#include <gslib.h>
//#include"Actor/Enemy/Editor/EditorDefine.h"
//#include"Util/RowsManager.h"
//#include"GTLib/Util/DiveineRow.h"
//using namespace std;
//
//namespace EditorHandleMessageTag {
//	extern const string GetObjectPlayerData;//描画オブジェの情報を得る
//	extern const string ChangeDrawMeshAnimation;//描画オブジェのアニメーションを変更する
//}
//
//const struct ObjectPlayerData {
//	//描画の基本情報
//	unsigned int assets_id_ = 0;
//	string name_ = "ObjectPlayer";
//	int draw_layer_ = 0;
//	Vector3 position_{0.0f,0.0f,0.0f};
//	Vector3 scale_{ 1.0f,1.0f,1.0f };
//	//アニメーション
//	int animation_num_max_ = 0;
//	//アニメーションレイヤー
//	unsigned int layer_num_ = 1;
//	std::vector<std::vector<unsigned int>> bone_layer_;
//};
//
//class IEditor {
//public:
//	virtual void update(float time=0.0f);
//	virtual void clear();
//	virtual void draw()const;
//	virtual void handle_message(const std::string& message, void* param);//メッセージ処理
//	Rows result()const { return result_; }
//	string key()const { return key_; }
//	bool is_closed()const { return is_closed_; }
//	void output_result();//書き出し用データに変換
//	Row cut_data(string data);
//	int judge_data(const int data_count, RowsManager* data, Row rule);//一行を分解しウィンドウに変換
//	int add_contents(int row_num, const int rule_row_num, Row rule, const bool is_new_rule);//ルールをもとにウィンドウに要素を追加
//	void input_ini_data(const int row_num, string id, string data, const bool is_add_id);//指定したデータを保存
//	bool baseic_data(const int row_num, int& j, string id, const bool is_add_id);//基本的なデータを保存
//	bool make_window(const int data_count, Row data_list, Row rule, string id, int& i, int& j, char ui_id[]);//データをもとに編集用ウィンドウを生成
//	void creat_ini_data(const int row_count);
//protected:
//	string key_ = "";
//	Rows result_;//最終的にテキストファイルに保存するやつ
//	bool is_closed_{ false };
//	RowsManager data_;
//	RowsManager rule_;
//	int element_add_id_{ 0 };//要素追加する行
//	int add_function_id_{ 0 };//機能追加に使うやつ
//	int new_rule_count_{ 0 };
//	bool is_ini_{ false };//初期化フラグ
//	vector<int> before_function_id_;//ファンクションＩＤの変更をチェックするやつ
//	vector<DevineRow> new_rule_;
//};
//
//#endif