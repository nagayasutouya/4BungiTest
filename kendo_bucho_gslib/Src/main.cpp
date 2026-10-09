#include "GTLib/GTLib.h"
#include "SceneManager.h"
#include "GamePlayScene.h"
#include "TitleScene.h"

#include "Sibungi/FPSCheck.h"

class MyGame : public Game {
public:
    // コンストラクタ
    MyGame() : Game{ 1024, 768 } {}
    // 開始
    void start() override {
        // グラフィックスシステムの初期化
        Graphics::initialize(graphics_device_);
        // パイプラインステートの基本設定
        PipelineSetup::setup_defaults();
        scene_manager_.add("TitleScene", new TitleScene());
        scene_manager_.add("GamePlayScene", new GamePlayScene());
        scene_manager_.change("TitleScene");
    }
    //　更新
    void update(float delta_time) override {
        fps_.Instance()->update();
        if (Input::get_key_down('P'))stop_ = (stop_) ? false : true;
        if (stop_)return;
        // ワールドクラスの更新
        float delta_frame = delta_time * 60.0f;
        scene_manager_.update(delta_frame);
    }
    // 描画
    void draw() override {
        if (stop_)return;
        // シーン開始
        Graphics::begin_scene(0.5f, 0.5f, 1.0f, 1.0f);
        scene_manager_.draw();
        // シーン終了
        Graphics::end_scene();
    }
    // 終了
    void end() override {
        scene_manager_.end();
    }

private:
    // シーンマネージャー
    SceneManager scene_manager_;
    bool stop_{false};
    //FPS
    FPSCheck fps_;
};

int main() {
    return MyGame().run();
}

