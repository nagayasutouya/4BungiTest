//#ifndef EDITOR_MANAGER_H_
//#define EDITOR_MANAGER_H_
//
//#include <vector>
//#include <string>
//#include <list>
//#include "GTLib/Util/CSVReader.h"
//#include "Actor/Enemy/Editor/EditorDefine.h"
//#include "GTLib/Util/DiveineRow.h"
//#include "GTLib/Data/DefineVector.h"
//using namespace std;
//class Editor;
//
////ながそんのデータ編集の総括場所
//class EditorManager {
//public:
//	EditorManager();
//	~EditorManager();
//	void update();
//	void draw()const;
//	void clear();
//private:
//	void add_window(int type, string name, int key, string key_name);
//	void load_data(const int rule_id);
//	void save();
//	void load_rule(const int rule_id);
//private:
//	CSVReader csv_;
//	list<Editor*>editors_;
//	int add_type_{ 0 };
//	bool is_on_awake_{ false };
//
//	bool is_set_file_{ false };//準備ができたら他のコマンドを描画
//	string file_name_ = "";
//	string file_pass_ = "";
//
//	DevineRow current_data_;
//	vector<int>window_count_;
//
//	//ルールの保存場所
//	vector<string> rules_path_;//読み込みに使うパス
//	VInt rule_genre_count_;//ジャンルの数
//	vector<DevineRow> rules_inf_;//ルールの保存先
//	Rows rule_genre_list_;//ルールが内包するジャンルのリスト
//	vector<VInt> rule_genre_id_list_;//ルールが内包するジャンルのロード側IDリスト
//	vector<VInt> rule_genre_draw_id_list_;//ルールが内包するジャンルの描画側IDリスト
//	int rule_count_{ 0 };//読み込まれたルールの数
//	int current_select_rule_{ 0 };//選んでいるルール
//
//	enum TypeID {
//		TID_Min = -1,
//		TID_AnimationLayer,
//		TID_AnimationEvent,
//		TID_Status,
//		TID_DrawActor,
//		TID_Collider,
//		TID_State,
//		TID_Judge,
//		TID_Function,
//		TID_Max,
//	};
//};
//
//#endif