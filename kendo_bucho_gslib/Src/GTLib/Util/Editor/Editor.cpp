//#include"Editor.h"
//#include"imgui/imgui.h"
//#include"Actor/Enemy/State/manager/EnemyStateManager.h"
//#include<iostream>
//#include<fstream>
//#include<sstream>
//
////複数ルールに変更する
//Editor::Editor(string name, string key, Rows rule, Rows data, vector<DevideRow> new_rule) :name_{ name }/*, data_{ data }, rule_{ rule }, new_rule_{ new_rule }*/ {
//	key_=key;
//	data_ = data;
//	rule_ = rule; 
//	new_rule_ = new_rule;
//	if (data.size() <= 0) {
//		if (rule_.rows() <= (unsigned)1)data_.add_row();
//		creat_ini_data(0);
//	}
//}
//
//void Editor::update(float time) {
//	ImGui::Begin(name_.c_str());
//	for (int i = 0; i < data_.rows(); i++) {//データ周回
//		int rule_row = (unsigned)i % rule_.rows();//行に合わせたルールを展開しないといけない
//		int result = judge_data(i, &data_, rule_.get_row(rule_row));
//	}
//	ImGui::PushID("ElemetID");
//	ImGui::SliderInt("ElemetID", &element_add_id_, 0, data_.rows() - 1);
//	ImGui::PopID();
//	ImGui::PushID("AddElement");
//	if (ImGui::Button("AddElement")) {
//		int c = rule_.rows();
//		int before_data_rows = element_add_id_;
//		add_contents(before_data_rows, 1, rule_.get_row(before_data_rows % c), false);
//	}
//	ImGui::PopID();
//	ImGui::PushID("AddContents");
//	if (ImGui::Button("AddContents")) {
//		int before_data_rows = data_.rows() - 1;
//		creat_ini_data(before_data_rows);
//	}
//	ImGui::PopID();
//	ImGui::PushID("Close");
//	if (ImGui::Button("Close/Can't Save")) {
//		is_closed_ = true;
//	}
//	ImGui::PopID();
//	result_ = data_.data();
//	ImGui::End();
//	new_rule_count_ = 0;
//	is_ini_ = true;//追加ルールの既存書き込みデータを読み込むタイミングを初回だけに限定　これによりスライドバーによるウィンドウの変化に対応
//}
//
//Row Editor::cut_data(string data) {
//	stringstream ss{ data };
//	string value;
//	Row row;
//	while (getline(ss, value, '_')) {
//		row.push_back(value);
//	}
//	return row;
//}
//
//void Editor::creat_ini_data(const int row_count) {
//	int c = rule_.rows();
//	for (int i = 0; i < c; i++) {
//		add_contents(row_count, c, rule_.get_row(i), false);
//	}
//}
//
////一行を分解しウィンドウに変換
//int Editor::judge_data(const int data_count, RowsManager* origin_data, Row rule) {
//	Row data_list = origin_data->get_row(data_count);
//	if (data_list.size() <= (unsigned)0 || rule.size() <= (unsigned)0) {
//		cout << data_count << "で空のデータが挿入された" << endl;
//		return -1;
//	}
//	int i = 0;
//	while ((unsigned)i < data_list.size()) {//データ周回
//		for (int j = 0; (unsigned)j < rule.size(); j++) {//ルール周回
//			string id = rule.at(j++);
//			char ui_id[512];
//			snprintf(ui_id, 512, "%d_%d_%d", data_count, i, j);
//			bool result = make_window(data_count, data_list, rule, id, i, j, ui_id);
//			if (result) {}
//			else if (id == MotionLoadID::NewRule) {
//				int function_id = stoi(data_list.at(i - 1));//一個前でIDを判断
//				int rule_id = stoi(rule.at(j++));
//				if (before_function_id_.size() < (unsigned)new_rule_count_ + 1) {
//					before_function_id_.push_back(function_id);
//				}
//				if (is_ini_ && before_function_id_.at(new_rule_count_) != function_id) {//データが変わっているかを調べ、変わってたらコンテナ作りなおし
//					origin_data->clear_row(data_count);
//					Row change_rule = new_rule_.at(rule_id).get_row(function_id, 0);
//					data_.add_data(data_count, to_string(function_id));
//					add_contents(data_count, 1, change_rule, true);
//					data_list = origin_data->get_row(data_count);
//					before_function_id_.at(new_rule_count_) = function_id;
//				}
//				new_rule_count_++;
//				Row rule2 = new_rule_.at(rule_id).get_row(function_id, 0);
//				for (int j2 = 0; (unsigned)j2 < rule2.size(); j2++) {//ルール周回
//					string id_from_data = rule2.at(j2);
//					char ui_id2[512];
//					snprintf(ui_id2, 512, "New_%d_%d_%d", data_count, i, j2);
//					if (id_from_data != MotionLoadID::Struct)data_.set(data_count, i++, id_from_data);
//					j2++;
//					if (!make_window(data_count, data_list, rule2, id_from_data, i, j2, ui_id2)) {
//						cout << data_count << "で謎の新規ルールが追加された" << endl;
//						return 0;
//					}
//				}
//			}
//			else {
//				Row devine_rule = cut_data(id);
//				if (devine_rule.size() <= 0) {
//					cout << data_count << "で規定外のデータが挿入された" << endl;
//					return 0;
//				}
//				else {
//					judge_data(data_count, origin_data, devine_rule);
//					return 2;
//				}
//			}
//		}
//		//１セットおわり
//		ImGui::Text("");
//	}
//	return 1;
//}
//
////データをもとに編集用ウィンドウを生成
//bool Editor::make_window(const int data_count, Row data_list, Row rule, string id, int& i, int& j, char ui_id[]) {
//	bool result = true;
//	if (id == MotionLoadID::Int) {
//		int data = stoi(data_list.at(i));
//		ImGui::PushID(ui_id);
//		ImGui::SliderInt(rule.at(j).c_str(), &data, stoi(rule.at(j + 1)), stoi(rule.at(j + 2)));
//		ImGui::PopID();
//		j += 2;
//		data_.seti(data_count, i++, data);
//	}
//	else if (id == MotionLoadID::Bool) {
//		int data = stoi(data_list.at(i));
//		ImGui::PushID(ui_id);
//		ImGui::SliderInt(rule.at(j).c_str(), &data, 0, 1);
//		ImGui::PopID();
//		data_.seti(data_count, i++, data);
//	}
//	else if (id == MotionLoadID::Float) {
//		float data = stof(data_list.at(i));
//		ImGui::PushID(ui_id);
//		ImGui::SliderFloat(rule.at(j).c_str(), &data, stof(rule.at(j + 1)), stof(rule.at(j + 2)));
//		ImGui::PopID();
//		j += 2;
//		data_.setf(data_count, i++, data);
//	}
//	else if (id == MotionLoadID::String) {
//		char s[512];
//		snprintf(s, 512, data_list.at(i).c_str());
//		ImGui::PushID(ui_id);
//		ImGui::InputText(rule.at(j).c_str(), s, 512);
//		ImGui::PopID();
//		data_.set(data_count, i++, s);
//	}
//	else if (id == MotionLoadID::Vector3) {
//		float data[3] = { stof(data_list.at(i)),stof(data_list.at(i + 1)),stof(data_list.at(i + 2)) };
//		ImGui::PushID(ui_id);
//		ImGui::SliderFloat3(rule.at(j).c_str(), data, stof(rule.at(j + 1)), stof(rule.at(j + 2)));
//		ImGui::PopID();
//		for (int k = 0; k < 3; k++) {
//			data_.setf(data_count, i++, data[k]);
//		}
//		j += 2;
//	}
//	else if (id == MotionLoadID::Text) {
//		string data = rule.at(j);
//		if (data == "DrawUI-ID") {
//			data = ui_id;
//		}
//		ImGui::PushID(ui_id);
//		ImGui::Text(data.c_str());
//		ImGui::PopID();
//	}
//	else if (id == MotionLoadID::Struct) {
//		data_.set(data_count, i++, rule.at(j++));
//		int elements_num = stoi(rule.at(j++));
//		for (int k = 0; k < elements_num; k++) {
//			string id2 = rule.at(j++);
//			char ui_id2[512];
//			snprintf(ui_id2, 512, "Struct_%d_%d_%d", data_count, i, k);
//			make_window(data_count, data_list, rule, id2, i, j, ui_id2);
//			j++;
//		}
//		j--;
//	}
//	else {
//		result = false;
//	}
//	return result;
//}
//
////ルールをもとにウィンドウに要素を追加
//int Editor::add_contents(int row_num, const int rule_row_num, Row rule, const bool is_new_rule) {
//	if (rule_row_num > 1) {//行追加
//		data_.add_row();
//		row_num = data_.rows() - 1;
//	}
//	for (int j = 0; (unsigned)j < rule.size(); j++) {//中身追加
//		string id = rule.at(j++);
//		if (baseic_data(row_num, j, id, is_new_rule)) {}
//		else if (id == MotionLoadID::Text) {/*データ書き込まないからスルー*/ }
//		else if (id == MotionLoadID::NewRule) {
//			int function_id = add_function_id_;
//			int rule_id = stoi(rule.at(j++));
//			Row rule2 = new_rule_.at(rule_id).get_row(function_id, 0);
//			add_contents(row_num, (unsigned)new_rule_.at(rule_id).rows(function_id), rule2, true);
//		}
//		else if (id == MotionLoadID::Struct) {
//			input_ini_data(row_num, id, rule.at(j++), false);
//			int struct_size = stoi(rule.at(j++));
//			for (int k = 0; k < struct_size; k++) {
//				id = rule.at(j++);
//				baseic_data(row_num, j, id, false);
//				j++;
//			}
//			j--;
//		}
//		else {
//			Row devine_rule = cut_data(id);
//			if (devine_rule.size() <= 0) {
//				cout << row_num << "で規定外のデータが挿入された" << endl;
//				return 0;
//			}
//			else {
//				add_contents(row_num, 1, devine_rule, false);
//				return 2;
//			}
//		}
//	}
//	return 1;
//}
//
////基本的なデータを保存
//bool Editor::baseic_data(const int row_num, int& j, string id, const bool is_new_rule) {
//	bool result = true;
//	if (id == MotionLoadID::Int || id == MotionLoadID::Float) {
//		input_ini_data(row_num, id, "0", is_new_rule);
//		j += 2;
//	}
//	else if (id == MotionLoadID::Bool) {
//		input_ini_data(row_num, id, "0", is_new_rule);
//	}
//	else if (id == MotionLoadID::String) {
//		input_ini_data(row_num, id, "a", is_new_rule);
//	}
//	else if (id == MotionLoadID::Vector3) {
//		if (is_new_rule)data_.add_data(row_num, id);
//		for (int k = 0; k < 3; k++) {
//			data_.add_data(row_num, "0");
//		}
//		j += 2;
//	}
//	else { result = false; }
//	return result;
//}
//
////指定したデータを保存
//void Editor::input_ini_data(const int row_num, string id, string data, const bool is_add_id) {
//	if (is_add_id)data_.add_data(row_num, id);
//	data_.add_data(row_num, data);
//}
////書き出し用データに変換
//void Editor::output_result() {
//	Rows data = result_;
//	for (int i = 0; (unsigned)i < data.size(); i++) {
//		for (int j = 0; (unsigned)j < data.at(i).size(); j++) {
//			data.at(i).at(j) += ',';
//		}
//	}
//	result_ = data;
//}