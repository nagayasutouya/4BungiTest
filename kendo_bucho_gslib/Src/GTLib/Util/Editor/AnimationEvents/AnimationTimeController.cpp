//#include"AnimationTimeController.h"
//#include"Rendering/AnimatedMesh.h"
//#include"Util/Util.h"
//#include"UniversalClass/DefineUnibersal.h"
//#include"imgui/imgui.h"
//#include<gsTransform.h>
//#include <GSeffect.h>
//
//AnimationTimeController::AnimationTimeController(string name,unsigned int id,string actor_name,Rows data) :editor_name_{ name }, event_editor_name_{name+"-EventList"} {
//	//初期化
//	owner_name_=actor_name;
//	load_data(data);
//	edit_target_ = new AnimatedMesh(id);
//	Transform tr;
//	tr.position({ 0.0f,0.0f,0.0f });
//	edit_target_->transform(tr.localToWorldMatrix());
//	event_genre_.push_back("SE");
//	event_genre_.push_back("BGM");
//	event_genre_.push_back("Effect");
//	log_.set_propaty(UPath::LogFilePath + log_.creat_file_name(owner_name_+"AnimationEvent"), true);
//}
//
//AnimationTimeController::~AnimationTimeController(){
//	save();
//	clear();
//}
//
//void AnimationTimeController::update(float time){
//	if (edit_target_ == nullptr)return;
//
//	//ここからエディタ
//	bool is_edit = false;
//	ImGui::Begin(editor_name_.c_str());
//	//ループ中かどうかだす
//	char s[512];
//	snprintf(s, sizeof(s), "%s",(loop_)?"Loop:ON":"Loop:OFF");
//	ImGui::Text(s);
//	//ループフラグ
//	ImGui::PushID("LoopAnimation");
//	if (ImGui::Button("LoopAnimation"))loop_ = (loop_) ? false : true;
//	ImGui::PopID();
//
//	//再生中かどうかだす
//	snprintf(s, sizeof(s), "%s", (play_) ? "Play:ON" : "Play:OFF");
//	ImGui::Text(s);
//	//再生フラグ
//	ImGui::PushID("PlayAnimation");
//	if (ImGui::Button("Play"))play_=(play_)?false:true;
//	ImGui::PopID();
//
//	//ここからメッシュ
//	//アニメーションの番号をコントロール
//	ImGui::PushID("CurrentAnimation");
//	is_edit=(!is_edit)?ImGui::InputInt("CurrentAnimation", &current_animation_):true;
//	ImGui::PopID();
//
//	//アニメーションのレイヤーをコントロール
//	ImGui::PushID("CurrentAnimationLayer");
//	is_edit = (!is_edit) ? ImGui::InputInt("CurrentAnimationLayer", &current_animation_layer_) :true;
//	ImGui::PopID();
//
//	//アニメーションの時間をコントロール
//	ImGui::PushID("CurrentTimeBar");
//	is_edit = (!is_edit) ? ImGui::SliderFloat("CurrentAnimationTime", &current_time_, 0.0f,end_time_) : true;
//	ImGui::PopID();
//	
//	//ここからアニメーションイベント
//	if (!play_) {
//		string t = "EventType:";
//		t += event_genre_.at(current_event_type_);
//		ImGui::PushID("EventType");
//		ImGui::Text(t.c_str());
//		ImGui::SliderInt("##EventType", &current_event_type_, 0, event_genre_.size() - 1);
//		ImGui::PopID();
//
//		//アニメーションイベントに合わせてウィンドウを生成
//		EventData data = set_event_data(current_event_type_, current_animation_, current_animation_layer_, current_time_);
//		//テスト用アニメーションイベント保存
//		if (ImGui::Button("Apply")) {
//			all_event_.push_back(data);
//		}
//	}
//	
//	ImGui::End();
//	//追加済みのイベント編集
//	ImGui::Begin(event_editor_name_.c_str());
//	//イベントがあったら描画
//	draw_event_data(all_event_);
//	ImGui::End();
//
//	//さくじょ
//	for (auto i = all_event_.begin(); i != all_event_.end();) {
//		if ((*i).is_delete_) {
//			i = all_event_.erase(i);
//		}
//		else {
//			++i;
//		}
//	}
//
//	//メッシュ動かす
//	if (play_) {
//		float prev_timer = current_time_;
//		update_mesh(time);
//		gsUpdateEffect(time);
//		//イベント再生
//		for (const auto& event : all_event_) {
//			if (event.event_.motion_id_ != current_animation_) continue;
//			if (prev_timer < event.event_.action_time_ && event.event_.action_time_ <= current_time_) {
//				play_event(event);
//			}
//		}
//	}
//
//	//メッシュに適応
//	if (!is_edit)return;
//	//モーション変更
//	edit_target_->change_motion(current_animation_,loop_,current_animation_layer_);
//	edit_target_->current_motion_time(current_time_,current_animation_layer_);
//	//終了時間の取得
//	end_time_ = edit_target_->motion_end_time(current_animation_layer_);
//	//動かす
//	update_mesh(time);
//}
//
//void AnimationTimeController::draw()const{
//	if (edit_target_ == nullptr)return;
//	edit_target_->draw();
//}
//
//void AnimationTimeController::clear() {
//	delete edit_target_;
//	edit_target_ = nullptr;
//}
//
//void AnimationTimeController::update_mesh(float time) {
//	edit_target_->update(time);
//	Transform tr;
//	tr.position({ 0.0f,0.0f,0.0f });
//	edit_target_->transform(tr.localToWorldMatrix());
//	//再生中だけバー動かす
//	if(play_)current_time_ = edit_target_->current_motion_time(current_animation_layer_);
//}
//
//AnimationTimeController::SEEventData AnimationTimeController::set_se_event(BaseEventData ved) {
//	static SEEventData data;
//	data.event_ = ved;
//	ImGui::InputInt("SEID",&data.id_);
//	return data;
//}
//
//AnimationTimeController::BGMEventData AnimationTimeController::set_bgm_event(BaseEventData ved){
//	static BGMEventData data;
//	data.event_ = ved; 
//	ImGui::InputInt("BGMID", &data.id_);
//	ImGui::SliderFloat("ChangeVolume", &data.change_,-1.0f,1.0f);
//	ImGui::SliderFloat("MaxVolume", &data.max_, 0.0f, 1.0f);
//	return data;
//}
//
//AnimationTimeController::EffectEventData AnimationTimeController::set_effect_event(BaseEventData ved){
//	static EffectEventData data;
//	data.event_=ved;
//	data.name_ = owner_name_;
//	ImGui::InputInt("EffectID", &data.id_);
//	ImGui::InputFloat3("Position", data.position_);
//	ImGui::InputFloat3("Rotation", data.rotation_);
//	ImGui::InputFloat3("Scale", data.scale_);
//	return data;
//}
//
//AnimationTimeController::EventData AnimationTimeController::set_event_data(int event_id, int motion_id,int layer, float time) {
//	static EventData data;
//	data.event_id_ = event_id;
//	data.event_.action_time_ = time;
//	data.event_.motion_id_ = motion_id;
//	data.event_.layer_ = layer;
//	switch (event_id) {
//	case ETID_BGM:
//		data.bgm_ = set_bgm_event(data.event_);
//		break;
//	case ETID_SE:
//		data.se_ = set_se_event(data.event_);
//		break;
//	case ETID_Effect:
//		data.effect_ = set_effect_event(data.event_);
//		break;
//	default:
//		break;
//	}
//	return data;
//}
//
//void AnimationTimeController::play_event(EventData data){
//	switch (data.event_id_) {
//	case ETID_BGM: {
//		break;
//	}
//	case ETID_SE: {
//		gsPlaySE(data.se_.id_);
//		break;
//	}
//	case ETID_Effect: {
//		Vector3 pos{ data.effect_.position_[0],data.effect_.position_[1] ,data.effect_.position_[2] };
//		Vector3 rot{ data.effect_.rotation_[0],data.effect_.rotation_[1] ,data.effect_.rotation_[2] };
//		Vector3 scale{ data.effect_.scale_[0],data.effect_.scale_[1] ,data.effect_.scale_[2] };
//		GSmatrix4 mat = GSmatrix4::TRS(pos, GSquaternion::euler(rot), scale);
//		gsPlayEffectEx(data.effect_.id_,&mat);
//		break;
//	}
//	default:
//		break;
//	}
//}
//
//void AnimationTimeController::draw_se_event(SEEventData data, int row){
//	char s[512];
//	snprintf(s, sizeof(s), "%d:%f SE ID:%d",row,data.event_.action_time_,data.id_);
//	ImGui::Text(s);
//}
//
//void AnimationTimeController::draw_bgm_event(BGMEventData data, int row){
//	char s[512];
//	snprintf(s, sizeof(s), "%d:%f BGM ID:%d", row, data.event_.action_time_, data.id_);
//	ImGui::Text(s);
//}
//
//void AnimationTimeController::draw_effect_event(EffectEventData data, int row){
//	char s[512];
//	snprintf(s, sizeof(s), "%d:%f Effect ID:%d", row, data.event_.action_time_, data.id_);
//	ImGui::Text(s);
//}
//
//void AnimationTimeController::draw_event_data(OneFlameEventList& list){
//	int row = 0;
//	static int add_id[3] = { 0,500,1000 };
//	//ソートできるようにしたいな
//	for (auto& event : list) {
//		char s[32];
//		ImGui::PushID(row+add_id[0]);
//		snprintf(s, sizeof(s), "MotionID:%d", event.event_.motion_id_);
//		ImGui::Text(s);
//		ImGui::PopID();
//		ImGui::PushID(row + add_id[1]);
//		switch (event.event_id_) {
//		case ETID_BGM: {
//			draw_bgm_event(event.bgm_,++row);
//			break;
//		}
//		case ETID_SE: {
//			draw_se_event(event.se_,++row);
//			break;
//		}
//		case ETID_Effect: {
//			draw_effect_event(event.effect_,++row);
//			break;
//		}
//		default:
//			ImGui::Text("Unknown Data");
//			break;
//		}
//		ImGui::PopID();
//		ImGui::PushID(row + add_id[2]);
//		if (ImGui::Button("Delete")) {
//			event.is_delete_ = true;
//		}
//		ImGui::PopID();
//	}
//}
//
//void AnimationTimeController::load_data(Rows data){
//	RowsManager mana(data);
//	for (int i = 0; i < (signed)data.size(); i++) {
//		//アセット番号の読み込み
//		string check = mana.get(i, 0);
//		//対象と違うのはスキップ
//		if (check != owner_name_) {
//			i++; 
//			continue;
//		}
//		//仕様タイミングを設定
//		BaseEventData bed;
//		bed.motion_id_ = mana.geti(i, 1);
//		bed.action_time_ = mana.getf(i++, 2);
//		//改行
//		//イベント詳細
//		bool is_known = true;
//		EventData ed;
//		ed.event_ = bed;
//		ed.event_id_ = mana.geti(i, 0);
//		switch (ed.event_id_) {
//		case ETID_BGM: {
//			ed.bgm_.event_ = bed;
//			ed.bgm_.id_ = mana.geti(i, 2);
//			ed.bgm_.change_ = mana.geti(i, 4);
//			ed.bgm_.max_ = mana.geti(i, 6);
//			break;
//		}
//		case ETID_SE: {
//			ed.se_.event_ = bed;
//			ed.se_.id_ = mana.geti(i, 2);
//			break;
//		}
//		case ETID_Effect: {
//			ed.effect_.event_ = bed;
//			ed.effect_.id_ = mana.geti(i, 4);
//			ed.effect_.position_[0] = mana.getf(i, 6);
//			ed.effect_.position_[1] = mana.getf(i, 7);
//			ed.effect_.position_[2] = mana.getf(i, 8);
//			ed.effect_.rotation_[0] = mana.getf(i, 10);
//			ed.effect_.rotation_[1] = mana.getf(i, 11);
//			ed.effect_.rotation_[2] = mana.getf(i, 12);
//			ed.effect_.scale_[0] = mana.getf(i, 14);
//			ed.effect_.scale_[1] = mana.getf(i, 15);
//			ed.effect_.scale_[2] = mana.getf(i, 16);
//			break;
//		}
//		default:
//			is_known = false;
//			break;
//		}
//		//追加
//		if(is_known)all_event_.push_back(ed);
//	}
//}
//
//void AnimationTimeController::save() {
//	for (const auto& event : all_event_) {
//		char buf[512];
//		//イベント詳細
//		switch (event.event_id_) {
//		case ETID_BGM: {
//			snprintf(buf, sizeof(buf), "%s,%d,%f", owner_name_.c_str(), event.event_.motion_id_, event.event_.action_time_);
//			log_.creat_log(buf, false);
//			snprintf(buf, sizeof(buf), "%d,Int,%d,Float,%f,Float,%f", event.event_id_, event.bgm_.id_, event.bgm_.change_, event.bgm_.max_);
//			log_.creat_log(buf, false);
//			break;
//		}
//		case ETID_SE: {
//			snprintf(buf, sizeof(buf), "%s,%d,%f", owner_name_.c_str(), event.event_.motion_id_, event.event_.action_time_);
//			log_.creat_log(buf, false);
//			snprintf(buf, sizeof(buf), "%d,Int,%d", event.event_id_, event.se_.id_);
//			log_.creat_log(buf, false);
//			break;
//		}
//		case ETID_Effect: {
//			snprintf(buf, sizeof(buf), "%s,%d,%f", owner_name_.c_str(), event.event_.motion_id_, event.event_.action_time_);
//			log_.creat_log(buf, false);
//			snprintf(buf, sizeof(buf), "%d,Int,%d,Vector3,%f,%f,%f,Vector3,%f,%f,%f,Vector3,%f,%f,%f", event.event_id_, event.effect_.id_, event.effect_.position_[0], event.effect_.position_[1], event.effect_.position_[2], event.effect_.rotation_[0], event.effect_.rotation_[1], event.effect_.rotation_[2], event.effect_.scale_[0], event.effect_.scale_[1], event.effect_.scale_[2]);
//			log_.creat_log(buf, false);
//			break;
//		}
//		default:
//			break;
//		}
//	}
//}