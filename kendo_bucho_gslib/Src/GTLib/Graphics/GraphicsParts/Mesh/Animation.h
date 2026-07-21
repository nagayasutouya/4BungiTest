#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include <iosfwd>
#include "GTLib/Math/Vector3.h"
#include "GTLib/Math/Quaternion.h"
#include "GTLib/Math/Matrix4x4.h"

// アニメーションクラス
class Animation {
public:
    struct KeyFrame {                           // キーフレーム構造体
        float                   frame_no;       // フレーム番号
        Vector3                 scale;          // 拡大縮小キー
        Quaternion              rotation;       // 回転キー
        Vector3                 position;       // 平行移動キー
        // 変換行列を計算する
        Matrix4x4               matrix() const;
        // キーフレームの補間
        static KeyFrame lerp(const KeyFrame& k1, const KeyFrame& k2, float weight);
    };
public:
    // コンストラクタ
    Animation() = default;
    // ファイル読み込み (ストリームから単一のアニメーションを読み込む)
    void load(std::istream& stream);
    // 指定ボーンのキーフレームデータ取得
    KeyFrame get_key_frame(const std::string& bone_name, float frame_no) const;
    // 終了フレーム数を返す
    float last_frame() const;
    // 消去
    void clear();

private:
    // キーフレームの区間を検索
    std::pair<int, int> find_key_frame_segment(
        const std::vector<KeyFrame>& keys, float frame_no) const;

private:
    // ボーンごとのキーフレーム
    std::unordered_map<std::string, std::vector<KeyFrame>> bone_key_frames_;
    // 終了フレーム
    float last_frame_{ 0.0f };
    //アニメーション名
    std::string name_{ "" };
};

#endif