//#ifndef ANIMATION_TIME_CONTROLLER_H_
//#define ANIMATION_TIME_CONTROLLER_H_
//
//#include "GTLib/Util/Editor/IEditor.h"
//#include "GTLib/Data/GlobalData_.h"
//#include <unordered_map>
//#include "GTLib/Util/DiveineRow.h"
//#include "GTLib/Data/DefineVector.h"
//#include "GTLib/Util/SystemLog/CreatLog.h"
//class AnimatedMesh;
//
//class AnimationTimeController {
//public:
//	AnimationTimeController() = default;
//	AnimationTimeController(string name, unsigned int id, string actor_name, Rows data);
//	~AnimationTimeController();
//	void update(float time);
//	void draw()const;
//	void clear();
//private:
//	enum EventTypeID {
//		ETID_None = -1,
//		ETID_SE,
//		ETID_BGM,
//		ETID_Effect,
//	};
//	//共通部分 アクターネームはedit_target_data_のname
//	struct BaseEventData{
//		int motion_id_ = -1;
//		float action_time_ = -1.0f;
//		int layer_ = 0;
//	};
//	struct SEEventData{
//		BaseEventData event_;
//		int id_ = 0;
//	};
//	struct BGMEventData{
//		BaseEventData event_;
//		int id_ = 0;
//		float change_ = 0.0f;
//		float max_ = 1.0f;
//	};
//	struct EffectEventData{
//		BaseEventData event_;
//		int id_ = 0;
//		string name_ = "";//edit_target_data_のname
//		float position_[3]{ 0.0f,0.0f,0.0f };
//		float rotation_[3]{ 0.0f,0.0f,0.0f };
//		float scale_[3]{ 0.0f,0.0f,0.0f };
//	};
//	struct EventData {
//		int event_id_ = ETID_None;//外部データには保存しない
//		bool is_delete_ = false;//消すフラグ 外部データには保存しない
//		BaseEventData event_;
//		EffectEventData effect_;
//		BGMEventData bgm_;
//		SEEventData se_;
//	};
//private:
//	string editor_name_ = "";
//	string event_editor_name_ = "";
//	AnimatedMesh* edit_target_{ nullptr };//編集するやつ
//	ObjectPlayerData edit_target_data_;//編集するやつのデータ
//	CreatLog log_;
//	//メッシュ
//	float current_time_{ 0.0f };
//	int current_animation_{0};
//	int current_animation_layer_{0};
//	float end_time_{ 0.0f };
//	string owner_name_{ "" };
//	//再生
//	bool loop_{ false };
//	bool play_{ false };
//	//アニメーションイベント
//	Row event_genre_;
//	using OneFlameEventList = list<EventData>;//1つの時間に割り当てられるイベント
//	using OneMotionEventList =unordered_map<float, OneFlameEventList>;//1つのアニメーションに入れるイベント全部
//	using EventList= unordered_map<int,OneMotionEventList>;//アニメーションイベント全部
//	OneFlameEventList all_event_;//データ保存箇所
//	EventList events_list_;//アニメーションイベント
//	unordered_map<int,VFloat>event_time_list_for_play_;//再生時に一度だけ全体操作する。
//	int current_event_type_{0};//アニメーションイベントのID
//	unsigned int current_assets_id_{ 0 };//アセットID
//private:
//	//読み込み
//	void load_data(Rows data);
//	//保存
//	void save();
//	//データ編集
//	SEEventData set_se_event(BaseEventData ved);
//	BGMEventData set_bgm_event(BaseEventData ved);
//	EffectEventData set_effect_event(BaseEventData ved);
//	EventData set_event_data(int event_id,int motion_id,int layer ,float time);
//	//データ描画
//	void draw_se_event(SEEventData data,int row);
//	void draw_bgm_event(BGMEventData data, int row);
//	void draw_effect_event(EffectEventData data, int row);
//	void draw_event_data(OneFlameEventList& list);
//	//テスト
//	void play_event(EventData data);
//	void update_mesh(float time);
//};
//
//#endif