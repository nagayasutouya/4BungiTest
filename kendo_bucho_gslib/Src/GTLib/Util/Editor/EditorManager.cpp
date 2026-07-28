//#include"EditorManager.h"
//#include"Editor.h"
//#include"AnimationEvents/AnimationTimeController.h"
//#include"Data/GenreName.h"
//#include"imgui/imgui.h"
//#include"Actor/Enemy/State/manager/EnemyStateManager.h"
//#include<stdexcept>
//
//static const string DefaultFilePass{ "Assets/Boss/Data/" };
//static const string RuleFilePath{"Assets/Data/RulePath.txt"};
//
//EditorManager::EditorManager() {
//	////初期値
//	//file_name_ = "TreeGuardDraw.txt";
//	//file_pass_ = "Assets/Data/Draw/Boss/";
//	//file_name_ = "PlayerSwordData.txt";
//	//file_pass_ = "Assets/Player/Data/";
//	//描画以外もはいってるボスのやつ
//	//file_name_ = "TreeGuardData.txt";
//	//file_pass_ = "Assets/Boss/TreeGuard/Data/";
//	file_name_ = "PlayerSwordDraw.txt";
//	file_pass_ = "Assets/Data/Draw/Player/";
//	current_select_rule_ = 0;
//}
//
//EditorManager::~EditorManager() {
//	clear();
//}
//
//void EditorManager::update() {
//
//	if (gsGetKeyState(GKEY_TAB) && gsGetKeyTrigger(GKEY_UP))is_on_awake_ = (!is_on_awake_) ? true : false;
//	if (!is_on_awake_)return;
//
//	for (auto editor : editors_) {
//		editor->update();
//	}
//
//	//閉じられてるウィンドウは削除する
//	for (auto i = editors_.begin(); i != editors_.end();) {
//		if ((*i)->is_closed()) {
//			delete* i;
//			i = editors_.erase(i);
//		}
//		else {
//			++i;
//		}
//	}
//
//	if (!is_set_file_) {
//		ImGui::Begin("EditMenu");
//		//保存先を設定
//		const int ss = 512;
//		static char s[ss];
//		snprintf(s, ss, file_name_.c_str());
//		ImGui::InputText("FileName", s, ss);
//		file_name_ = s;
//		snprintf(s, ss, file_pass_.c_str());
//		ImGui::InputText("FilePass", s, ss);
//		file_pass_ = s;
//		ImGui::SliderInt("RuleID", &current_select_rule_, 0, 2);
//		//保存先を決定し編集に進む
//		if (ImGui::Button("Edit")) {
//			if (file_pass_ != "") {
//				load_data(current_select_rule_);
//			}
//			is_set_file_ = true;
//		}
//		ImGui::End();
//	}
//	else {
//		ImGui::Begin("EditMenu");
//		//指定したタイプのウィンドウを制作
//		string s = rule_genre_list_.at(0).at(add_type_);
//		ImGui::Text(s.c_str());
//		ImGui::SliderInt("##Choose", &add_type_, 0, (signed)rule_genre_list_.at(0).size() - 1);
//		if (ImGui::Button("Add")) {
//			int j = window_count_.at(add_type_);
//			char name_add[512];
//			snprintf(name_add, 512, "%d", j);
//			int id = rule_genre_draw_id_list_.at(0).at(add_type_) + j;
//			add_window(add_type_, s + name_add, id, s);
//		}
//		//保存する
//		if (ImGui::Button("Save")) {
//			is_set_file_ = false;
//			save();
//		}
//		ImGui::End();
//	}
//}
//
//void EditorManager::draw()const {
//	//for (auto editor: editors_) {
//	//	editor->draw();
//	//}
//}
//
//void EditorManager::add_window(int type, string name, int key, string key_name) {
//	Rows data;
//	if (current_data_.is_alive(key))data = current_data_.get_rows(key);
//	Editor* ed;
//	//if (key_name=="AnimationEvent") ed = new AnimationTimeController{name,key_name,rules_inf_.at(0).get_rows(type),data,rules_inf_};
//	/*else*/ ed = new Editor{ name,key_name,rules_inf_.at(0).get_rows(type),data,rules_inf_};//追加ルールの指定変更しなきゃいかん　それはeditorを変えて対応
//	window_count_.at(type)++;
//	editors_.push_back(ed);
//	ed = nullptr;
//}
//
//void EditorManager::load_data(const int rule_id) {
//	load_rule(rule_id);
//	current_data_.input(file_pass_ + file_name_, rule_genre_list_.at(0), rule_genre_draw_id_list_.at(0), true);
//	for (int i = 0; i < rule_genre_count_.at(0); i++) {
//		for (int j = 0; j < current_data_.ldt_id_size(i); j++) {
//			string s = rule_genre_list_.at(0).at(i);
//			char name_add[512];
//			snprintf(name_add, 512, "%d", j);
//			int id = rule_genre_draw_id_list_.at(0).at(i) + j;
//			if (current_data_.is_alive(id)) {
//				add_window(i, s + name_add, id, s);
//			}
//		}
//	}
//}
//
//void EditorManager::clear() {
//	for (auto i: editors_) {
//		delete i;
//	}
//	editors_.clear();
//}
//
//void EditorManager::save() {
//	Rows save_data;
//	for (auto editor : editors_) {
//		editor->output_result();
//		Row key;
//		key.push_back(editor->key());
//		save_data.push_back(key);
//		Rows data = editor->result();
//		for (int i = 0; (unsigned)i < data.size(); i++) { save_data.push_back(data.at(i)); }
//		key.clear();
//		key.push_back("#end");
//		save_data.push_back(key);
//	}
//	csv_.save(save_data, file_pass_ + file_name_);
//	clear();
//}
//
//void EditorManager::load_rule(const int rule_id){
//	csv_.load(RuleFilePath);
//	if (csv_.rows() <= rule_id)throw runtime_error("this id is not exist");
//	//前のルール削除
//	rule_genre_count_.clear();
//	rule_genre_draw_id_list_.clear();
//	rule_genre_id_list_.clear();
//	rule_genre_list_.clear();
//	rules_inf_.clear();
//	//ルールと必要要素の読み込み
//	rule_count_ = 0;
//	for (int col = 0; col < csv_.columns(rule_id); col++) {
//		string name = csv_.get(rule_id, col++);
//		rule_genre_count_.push_back(csv_.geti(rule_id, col++));
//		Row row;
//		VInt vint;
//		//保存先を生成
//		rule_genre_list_.push_back(row);
//		rule_genre_id_list_.push_back(vint);
//		rule_genre_draw_id_list_.push_back(vint);
//		//内容を保存
//		for (int i = 0; i < rule_genre_count_.at(rule_count_); i++) {
//			rule_genre_list_.at(rule_count_).push_back(csv_.get(rule_id, col++));
//			rule_genre_id_list_.at(rule_count_).push_back(i);
//			rule_genre_draw_id_list_.at(rule_count_).push_back(csv_.geti(rule_id, col++));
//			if(rule_count_==0)window_count_.push_back(0);
//		}
//		col--;
//		DevineRow drow;
//		drow.input(name, rule_genre_list_.at(rule_count_), rule_genre_id_list_.at(rule_count_), true);
//		rules_inf_.push_back(drow);
//		rule_count_++;
//	}
//}