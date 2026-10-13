#ifndef GTEFFECT_H_
#define GTEFFECT_H_

#include <EffekseerRendererDX12.Renderer.h>

#include <XAudio2.h>
#include <d3d12.h>
#include <vector>
#include <wrl/client.h>
#include "GTLib/GTLibMath.h"
#include "GTLib/Math/Color.h"

class GraphicsDevice;

//エフェクト管理クラス
class GTEffect {
public:
	//コンストラクタ
	GTEffect(GraphicsDevice* device, ID3D12CommandQueue* queue,int swap_buffer_count=1);
	//初期化
	void initialize(GTUINT effext_load_cout_max = 1024, GTUINT instance_count_max = 2048,DXGI_FORMAT render_format= DXGI_FORMAT_R8G8B8A8_UNORM,int render_target_count=1, DXGI_FORMAT depth_format= DXGI_FORMAT_D32_FLOAT,bool is_reversed_depth=false,int square_max_count=800);
	//エフェクトの読み込み
	bool load_effect(GTUINT effect_id, const char* file_name, float scale=1.0f);
	//エフェクトの再生(ハンドルが返ってくる)
	int play_effect(GTUINT effect_id, const Vector3* postion=nullptr);
	int play_effect_mat(GTUINT effect_id, const Matrix4x4* matrix=nullptr);
	// エフェクトの更新
	void update_effect(float frame_time);
	// エフェクト用のカメラの設定をする
	void set_effect_camera(Vector3 eye, Vector3 at, float y_angle=90.0f, Vector3 up=Vector3{0.0f,1.0f,0.0f});
	// エフェクトの描画開始
	void begin_rendering_effect();
	// エフェクトの描画終了
	void end_rendering_effect();
	// エフェクトの描画
	void draw_effect();
	// エフェクトのハンドル単位の描画
	void draw_effect(int handle);
	// エフェクトを停止する
	void stop_effect(int handle);
	// 全エフェクトを停止する
	void stop_all_effects();
	// エフェクトのルートだけを停止する
	void stop_root_effect(int handle);
	//エフェクト全部削除
	void finish_effect();
	//指定したエフェクトを破棄
	void delete_effect(GTUINT effect_id);
	// エフェクトのインスタンスが存在しているか調べる
	bool exists_effect(int handle);
	// エフェクトの色空間をガンマカラー空間にする
	void set_effect_color_space_type_gamma();
	// エフェクトの色空間をリニアカラー空間にする
	void set_effect_color_space_type_linear();
	// 透視変換行列の設定
	void set_effect_projection_matrix(const Matrix4x4* matrix);
	// 視野変換行列の設定
	void set_effect_view_matrix(const Matrix4x4* matrix);
	// エフェクトのインスタンスに変換行列を設定
	void set_effect_matrix(int handle, const Matrix4x4* matrix);
	// エフェクトのインスタンスに設定されている行列を取得
	void get_effect_matrix(int handle, Matrix4x4* result);
	// エフェクトのインスタンスの位置を取得
	void get_effect_position(int handle, Vector3* position);
	// エフェクトのインスタンスの位置を指定
	void set_effect_position(int handle, const Vector3* position);
	// エフェクトのインスタンスの回転角度を指定
	void set_effect_rotation_roll_pitch_yaw(int handle, float roll, float pitch, float yaw);
	// エフェクトのインスタンスの回転角度を指定（任意軸の周り回転）
	void set_effect_rotation_axis_angle(int handle, const Vector3* axis, float angle);
	// エフェクトのインスタンスの拡大率を指定
	void set_effect_scale(int handle, const Vector3* scale);
	//	エフェクトのインスタンスのターゲット位置を指定
	void set_effect_target_position(int handle, const Vector3* position);
	// エフェクトのベース変換行列を取得
	void get_effect_base_matrix(int handle, Matrix4x4* matrix);
	// エフェクトのベース変換行列を設定
	void set_effect_base_matrix(int handle, const Matrix4x4* mat);
	// エフェクトのインスタンスをDraw時に描画するか設定
	void set_effect_shown(int handle, bool shown);
	// エフェクトのインスタンスを一時停止または再開する
	void set_effect_paused(int handle, bool paused);
	// エフェクトのインスタンスの再生スピードを設定
	void set_effect_speed(int handle, float speed);
	// エフェクトのインスタンスのカラーを設定する
	void set_effect_color(int handle, const Color* color);
	// エフェクトがDrawで自動的に描画されるか設定する
	void set_effect_auto_drawing(int handle, bool autoDraw);
	//エフェクトを動かす
	void move_effect(int handle, Vector3 move);
	//フレームの最初に呼ぶ　エフェクトにフレームの開始を伝える
	void send_new_frame();
	//フレーム開始後のコマンドリストの再設定後に、エフェクトコマンドリストに設定を渡す
	void set_effect_command_list_param();
	//フレームの最後に呼ぶ　コマンドリストの終了
	void end_frame();
	//視点位置
	void set_viewer_position(Vector3 pos);
	//エフェクトの描画範囲をワールド座標基準に決める
	void set_effect_draw_zbuffer(float z_near, float z_far);
private:
	GraphicsDevice* device_{};// DirectX12デバイス
	ID3D12CommandQueue* queue_{};//コマンドキュー
	int swap_buffer_count_{ 0 };//スワップバッファの番号
	ID3D12Resource* texture_resource_{};//テクスチャのリソース
	Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool> memory_pool_;// メモリプールの作成
	Effekseer::RefPtr<EffekseerRenderer::CommandList> command_list_;// コマンドリストの作成
	Effekseer::Vector3D viewer_position_{0.0f,0.0f,0.0f};// 視点位置
	float z_near_{ 0.0f };//エフェクトの描画範囲の一番近いとこ
	float z_far_{ 0.0f };//エフェクトの描画範囲の一番遠いとこ
};

#endif