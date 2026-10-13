#include <xaudio2.h>
#include <vector>
#include "GTEffect.h"

#ifndef _DEBUG
#pragma comment(lib, "Effekseer.lib")
#pragma comment(lib, "EffekseerRendererDX12.lib")
#pragma comment(lib, "EffekseerRendererLLGI.lib")
#pragma comment(lib, "LLGI.lib")
#pragma comment(lib, "EffekseerRendererCommon.lib")
#else
#pragma comment(lib, "Effekseerd.lib")
#pragma comment(lib, "EffekseerRendererDX12d.lib")
#pragma comment(lib, "EffekseerRendererLLGId.lib")
#pragma comment(lib, "LLGId.lib")
#pragma comment(lib, "EffekseerRendererCommond.lib")
#endif
#include <Effekseer.h>
#include <EffekseerRendererDX12.h>
#include "GTLib/Graphics/GraphicsParts/GraphicsDevice.h"


static void InitEffects(GTUINT effectMaxCount);
static void ClearEffects();
static void ConvertMatrix4x4ToEffekseerMatrix44(const float* in, Effekseer::Matrix44* out);
static void ConvertEffekseerMatrix43ToMatrix4x4(const Effekseer::Matrix43* in, float* out);
static void ConvertMatrix4x4ToEffekseerMatrix43(const float* in, Effekseer::Matrix43* out);
static wchar_t* ConvertSjisToUTF16(const char* sjis);

// グラフィックデバイス
static Effekseer::Backend::GraphicsDeviceRef	g_graphicsDevice = nullptr;
// エフェクトマネージャー
static Effekseer::ManagerRef					g_manager = nullptr;
// エフェクトレンダラー
static EffekseerRenderer::RendererRef			g_renderer = nullptr;
// サウンド
//static EffekseerSound::SoundRef					g_sound = nullptr;
// ファイルインターフェース
static Effekseer::FileInterface* g_effectFile = nullptr;
// XAudio2
static IXAudio2* g_xa2 = nullptr;
// XAudio2マスターボイス
static IXAudio2MasteringVoice* g_xa2_master = nullptr;
// エフェクト配列
static std::vector<Effekseer::EffectRef>		g_effects;
// カラースペース
static Effekseer::ColorSpaceType				g_colorSpace = Effekseer::ColorSpaceType::Gamma;

//コンストラクタ
GTEffect::GTEffect(GraphicsDevice* device, ID3D12CommandQueue* queue, int swap_buffer_count) {
	device_ = device;
	queue_ = queue;
	swap_buffer_count_ = swap_buffer_count;
}
//初期化
void GTEffect::initialize(GTUINT effect_load_count_max,GTUINT instance_count_max,DXGI_FORMAT render_format,int render_target_count, DXGI_FORMAT depth_format,bool is_reversed_depth,int square_max_count){
	//エフェクト全部削除
	finish_effect();
	// グラフィックデバイスの作成
	g_graphicsDevice = EffekseerRendererDX12::CreateGraphicsDevice(device_->d3d12_device(), queue_, swap_buffer_count_);
	// 描画用インスタンスの生成
	g_renderer = EffekseerRendererDX12::Create(g_graphicsDevice, &render_format, render_target_count, depth_format, is_reversed_depth, square_max_count);
	//メモリプールの作成
	memory_pool_ = EffekseerRenderer::CreateSingleFrameMemoryPool(g_renderer->GetGraphicsDevice());
	//コマンドリストの作成
	command_list_ = EffekseerRenderer::CreateCommandList(g_renderer->GetGraphicsDevice(), memory_pool_);
	// エフェクト管理用インスタンスの生成
	g_manager = Effekseer::Manager::Create(instance_count_max);
	// 描画用インスタンスから描画機能を設定
	g_manager->SetSpriteRenderer(g_renderer->CreateSpriteRenderer());
	g_manager->SetRibbonRenderer(g_renderer->CreateRibbonRenderer());
	g_manager->SetRingRenderer(g_renderer->CreateRingRenderer());
	g_manager->SetModelRenderer(g_renderer->CreateModelRenderer());
	g_manager->SetTrackRenderer(g_renderer->CreateTrackRenderer());
	// 描画用インスタンスからテクスチャの読込機能を設定
	// 独自拡張可能、現在はファイルから読み込んでいる。
	g_manager->SetTextureLoader(g_renderer->CreateTextureLoader());
	g_manager->SetModelLoader(g_renderer->CreateModelLoader());
	g_manager->SetMaterialLoader(g_renderer->CreateMaterialLoader());
	g_manager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
	// 右手座標系を使用
	g_manager->SetCoordinateSystem(Effekseer::CoordinateSystem::RH);
	// エフェクト配列の初期化
	InitEffects(effect_load_count_max);
	//カメラパラメータの初期化
	Matrix4x4 m = Matrix4x4::identity();
	set_effect_view_matrix(&m);
	set_effect_projection_matrix(&m);
	set_effect_draw_zbuffer(0.01f, 10000.0f);
}
//エフェクト読み込み
bool GTEffect::load_effect(GTUINT effect_id, const char* file_name, float scale) {
	//前のがあったら消す
	delete_effect(effect_id);
	// UTF16のファイル名に変換
	const auto utf16_file_name = ConvertSjisToUTF16(file_name);
	// エフェクトファイルの読み込み
	g_effects[effect_id] = Effekseer::Effect::Create(g_manager, (const EFK_CHAR*)utf16_file_name, scale);
	delete[] utf16_file_name;
	return (g_effects[effect_id] != nullptr) ? true : false;
}
//エフェクトの再生(ハンドルが返ってくる)
int GTEffect::play_effect(GTUINT effect_id,const Vector3* position) {
	//無かったら無視
	if (effect_id > g_effects.size() || g_effects[effect_id] == nullptr) {
		return -1;
	}
	//座標を適応
	static const Vector3 zero_position = { 0.0f, 0.0f, 0.0f };
	const Vector3* init_position = &zero_position;
	if (position != NULL) {
		init_position = position;
	}
	return g_manager->Play(g_effects[effect_id], init_position->x, init_position->y, init_position->z);
}
//エフェクトの再生(ハンドルが返ってくる)
int GTEffect::play_effect_mat(GTUINT effect_id, const Matrix4x4* matrix) {
	//無かったら無視
	if (effect_id > g_effects.size() || g_effects[effect_id] == nullptr) {
		return -1;
	}
	//再生してマトリックスを変更
	auto handle = g_manager->Play(g_effects[effect_id], 0.0f, 0.0f, 0.0f);
	if (matrix != NULL) {
		set_effect_matrix(handle, matrix);
	}
	return handle;
}
// エフェクトの更新
void GTEffect::update_effect(float frame_time) {
	// レイヤーパラメータの設定
	Effekseer::Manager::LayerParameter layer_parameter;
	layer_parameter.ViewerPosition = viewer_position_;
	g_manager->SetLayerParameter(0, layer_parameter);
	g_manager->Update(frame_time);
	float time = g_renderer->GetTime() + frame_time * (1.0f / 60.0f);
	g_renderer->SetTime(time);
}
// エフェクト用のカメラの設定をする
void GTEffect::set_effect_camera(Vector3 eye, Vector3 at,float y_angle,Vector3 up) {
	// 投影行列を設定
	::Effekseer::Matrix44 projection_matrix;
	projection_matrix.PerspectiveFovRH(Mathf::deg_to_rad(y_angle), (float)device_->desc_data().window_width / (float)device_->desc_data().window_height, z_near_, z_far_);//垂直視野角、アスペクト比、描画を始めるカメラから一番近いとこ、描画を終えるカメラから一番遠いとこ
	g_renderer->SetProjectionMatrix(projection_matrix);
	// カメラ行列を設定
	Effekseer::Matrix44 camera_matrix;
	set_viewer_position(eye);
	camera_matrix.LookAtRH(viewer_position_, ::Effekseer::Vector3D(at.x, at.y, at.z), ::Effekseer::Vector3D(up.x, up.y, up.z));
	g_renderer->SetCameraMatrix(camera_matrix);
}
// エフェクトの描画開始
void GTEffect::begin_rendering_effect() {
	g_renderer->BeginRendering();
}
// エフェクトの描画終了
void GTEffect::end_rendering_effect() {
	g_renderer->EndRendering();
}
// エフェクトの描画
void GTEffect::draw_effect() {
	// エフェクト描画
	Effekseer::Manager::DrawParameter drawParameter;
	drawParameter.ZNear = z_near_;
	drawParameter.ZFar = z_far_;
	drawParameter.ViewProjectionMatrix = g_renderer->GetCameraProjectionMatrix();
	//命令部分のオープン
	set_effect_command_list_param();
	//描画開始
	begin_rendering_effect();
	//描画
	g_manager->Draw(drawParameter);
	//描画終了
	end_rendering_effect();
	//命令部分のクローズ
	end_frame();
}
// エフェクトのハンドル単位の描画
void GTEffect::draw_effect(int handle) {
	g_manager->DrawHandle(handle);
}
// エフェクトを停止する
void GTEffect::stop_effect(int handle) {
	g_manager->StopEffect(handle);
}
// 全エフェクトを停止する
void GTEffect::stop_all_effects() {
	g_manager->StopAllEffects();
}
// エフェクトのルートだけを停止する
void GTEffect::stop_root_effect(int handle) {
	g_manager->StopRoot(handle);
}
//エフェクト全部削除
void GTEffect::finish_effect(){
	// エフェクトの消去
	ClearEffects();
	// 先にエフェクト管理用インスタンスを破棄
	if(g_manager!=nullptr)g_manager.Reset();
	//コマンドリストを破棄
	if (command_list_ != nullptr)command_list_->Release();
	//メモリプールを破棄
	if (memory_pool_ != nullptr)memory_pool_->Release();
	// 次に描画用インスタンスを破棄
	if (g_renderer != nullptr)g_renderer.Reset();
	// 次にグラフィックデバイスを破棄
	if (g_graphicsDevice != nullptr)g_graphicsDevice.Reset();
}
//指定したエフェクトを破棄
void GTEffect::delete_effect(GTUINT effect_id) {
	g_effects[effect_id].Reset();
}
// エフェクトのインスタンスが存在しているか調べる
bool GTEffect::exists_effect(int handle) {
	return g_manager->Exists(handle) ? true : false;
}
// エフェクトの色空間をガンマカラー空間にする
void GTEffect::set_effect_color_space_type_gamma() {
	g_colorSpace = Effekseer::ColorSpaceType::Gamma;
}
// エフェクトの色空間をリニアカラー空間にする
void GTEffect::set_effect_color_space_type_linear() {
	g_colorSpace = Effekseer::ColorSpaceType::Linear;
}
// 透視変換行列の設定
void GTEffect::set_effect_projection_matrix(const Matrix4x4* matrix) {
	Effekseer::Matrix44 projction;
	ConvertMatrix4x4ToEffekseerMatrix44(&matrix->m[0][0], &projction);
	g_renderer->SetProjectionMatrix(projction);
}
// 視野変換行列の設定
void GTEffect::set_effect_view_matrix(const Matrix4x4* matrix) {
	Effekseer::Matrix44 view;
	ConvertMatrix4x4ToEffekseerMatrix44(&matrix->m[0][0], &view);
	g_renderer->SetCameraMatrix(view);
}
// エフェクトのインスタンスに設定されている行列を取得
void GTEffect::get_effect_matrix(int handle, Matrix4x4* result) {
	const auto& in = g_manager->GetMatrix(handle);
	ConvertEffekseerMatrix43ToMatrix4x4(&in, &result->m[0][0]);
}
// エフェクトのインスタンスに変換行列を設定
void GTEffect::set_effect_matrix(int handle, const Matrix4x4* matrix) {
	Effekseer::Matrix43 world;
	ConvertMatrix4x4ToEffekseerMatrix43(&matrix->m[0][0], &world);
	g_manager->SetMatrix(handle, world);
}
// エフェクトのインスタンスの位置を取得
void GTEffect::get_effect_position(int handle, Vector3* position) {
	const auto& location = g_manager->GetLocation(handle);
	position->x = location.X;
	position->y = location.Y;
	position->z = location.Z;
}
// エフェクトのインスタンスの位置を指定
void GTEffect::set_effect_position(int handle, const Vector3* position) {
	g_manager->SetLocation(handle, position->x, position->y, position->z);
}
// エフェクトのインスタンスの回転角度を指定
void GTEffect::set_effect_rotation_roll_pitch_yaw(int handle, float roll, float pitch, float yaw) {
	g_manager->SetRotation(handle, Mathf::deg_to_rad(pitch), Mathf::deg_to_rad(yaw), Mathf::deg_to_rad(roll));
}
// エフェクトのインスタンスの回転角度を指定（任意軸の周り回転）
void GTEffect::set_effect_rotation_axis_angle(int handle, const Vector3* axis, float angle) {
	g_manager->SetRotation(handle, Effekseer::Vector3D(axis->x, axis->y, axis->z), Mathf::deg_to_rad(angle));
}
// エフェクトのインスタンスの拡大率を指定
void GTEffect::set_effect_scale(int handle, const Vector3* scale) {
	g_manager->SetScale(handle, scale->x, scale->y, scale->z);
}
//	エフェクトのインスタンスのターゲット位置を指定
void GTEffect::set_effect_target_position(int handle, const Vector3* position) {
	g_manager->SetTargetLocation(handle, position->x, position->y, position->z);
}
// エフェクトのベース変換行列を取得
void GTEffect::get_effect_base_matrix(int handle, Matrix4x4* matrix) {
	const auto& in = g_manager->GetBaseMatrix(handle);
	ConvertEffekseerMatrix43ToMatrix4x4(&in, &matrix->m[0][0]);
}
// エフェクトのベース変換行列を設定
void GTEffect::set_effect_base_matrix(int handle, const Matrix4x4* mat) {
	Effekseer::Matrix43 world;
	ConvertMatrix4x4ToEffekseerMatrix43(&mat->m[0][0], &world);
	g_manager->SetBaseMatrix(handle, world);
}
// エフェクトのインスタンスをDraw時に描画するか設定
void GTEffect::set_effect_shown(int handle, bool shown) {
	g_manager->SetShown(handle, shown == true);
}
// エフェクトのインスタンスを一時停止または再開する
void GTEffect::set_effect_paused(int handle, bool paused) {
	g_manager->SetPaused(handle, paused == true);
}
// エフェクトのインスタンスの再生スピードを設定
void GTEffect::set_effect_speed(int handle, float speed) {
	g_manager->SetSpeed(handle, speed);
}
// エフェクトのインスタンスのカラーを設定する
void GTEffect::set_effect_color(int handle, const Color* color) {
	Effekseer::Color set_color(
		(uint8_t)(color->r * 255.0f),
		(uint8_t)(color->g * 255.0f),
		(uint8_t)(color->b * 255.0f),
		(uint8_t)(color->a * 255.0f));
	g_manager->SetAllColor(handle, set_color);
}
// エフェクトがDrawで自動的に描画されるか設定する
void GTEffect::set_effect_auto_drawing(int handle,bool autoDraw) {
	g_manager->SetAutoDrawing(handle, autoDraw == true);
}
//エフェクトを動かす
void GTEffect::move_effect(int handle, Vector3 move) {
	g_manager->AddLocation(handle, Effekseer::Vector3D(move.x,move.y,move.z));
}
//フレームの最初に呼ぶ　エフェクトにフレームの開始を伝える
void GTEffect::send_new_frame() {
	memory_pool_->NewFrame();
}
//フレーム開始後のコマンドリストの再設定後に、エフェクトコマンドリストに設定を渡す
void GTEffect::set_effect_command_list_param() {
	EffekseerRendererDX12::BeginCommandList(command_list_, device_->command_list());
	g_renderer->SetCommandList(command_list_);
}
//フレームの最後に呼ぶ　コマンドリストの終了
void GTEffect::end_frame() {
	g_renderer->SetCommandList(nullptr);
	EffekseerRendererDX12::EndCommandList(command_list_);
}
//視点位置
void GTEffect::set_viewer_position(Vector3 pos) {
	viewer_position_ = Effekseer::Vector3D{pos.x,pos.y,pos.z};
}
//エフェクトの描画範囲をワールド座標基準に決める
void GTEffect::set_effect_draw_zbuffer(float z_near, float z_far){
	z_near_ = z_near;
	z_far_ = z_far;
}

// SJISをUTF16に変換
static wchar_t* ConvertSjisToUTF16(const char* sjis) {
	// Shift-JIS文字列からUTF-16に変換したときの文字列長を求める
	const int length = MultiByteToWideChar(CP_ACP, 0, sjis, strlen(sjis), NULL, 0);
	// UTF-16文字列の領域を確保する。
	wchar_t* result = new wchar_t[length * 2 + 2];
	if (result != NULL) {
		// \0を代入
		result[length] = '\0';
		// Shift-JIS文字列からUTF-16に変換する。
		MultiByteToWideChar(CP_ACP, 0, sjis, strlen(sjis), result, length);
	}
	return result;
}
// エフェクト配列の初期化
static void InitEffects(GTUINT effectMaxCount) {
	// エフェクトのクリア
	ClearEffects();
	// エフェクト配列のリサイズ
	g_effects.resize(effectMaxCount, nullptr);
}
// エフェクト配列の消去
static void ClearEffects() {
	for (auto& effect : g_effects) {
		effect.Reset();
	}
	g_effects.clear();
}
// maitix4x4をEffekseerMatrix44に変換
static void ConvertMatrix4x4ToEffekseerMatrix44(const float* in, Effekseer::Matrix44* out) {
	memcpy(&out->Values[0][0], in, sizeof(out->Values));
}
// EffekseerMatrix43をmaitix4x4に変換
static void ConvertEffekseerMatrix43ToMatrix4x4(const Effekseer::Matrix43* in, float* out) {
	out[0] = in->Value[0][0]; out[1] = in->Value[0][1]; out[2] = in->Value[0][2]; out[3] = 0.0f;
	out[4] = in->Value[1][0]; out[5] = in->Value[1][1]; out[6] = in->Value[1][2]; out[7] = 0.0f;
	out[8] = in->Value[2][0]; out[9] = in->Value[2][1]; out[10] = in->Value[2][2]; out[11] = 0.0f;
	out[12] = in->Value[3][0]; out[13] = in->Value[3][1]; out[14] = in->Value[3][2]; out[15] = 1.0f;
}
// maitix4x4をEffekseerMatrix43に変換
static void ConvertMatrix4x4ToEffekseerMatrix43(const float* in, Effekseer::Matrix43* out) {
	out->Value[0][0] = in[0];  out->Value[0][1] = in[1];  out->Value[0][2] = in[2];
	out->Value[1][0] = in[4];  out->Value[1][1] = in[5];  out->Value[1][2] = in[6];
	out->Value[2][0] = in[8];  out->Value[2][1] = in[9];  out->Value[2][2] = in[10];
	out->Value[3][0] = in[12]; out->Value[3][1] = in[13]; out->Value[3][2] = in[14];
}