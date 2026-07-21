#include "Animation.h"
#include <fstream>
#include <algorithm>

// ファイル読み込み (ストリームから単一のアニメーションを読み込む)
void Animation::load(std::istream& file) {
    // 消去
    clear();
    //アニメーション名取得
    file >> name_;
    // ボーン数の読み込み
    int num_bone;
    file >> num_bone;
    for (int i = 0; i < num_bone; ++i) {
        // ボーン名の読み込み
        std::string bone_name;
        file >> bone_name;
        // キーフレーム数の読み込み
        int num_key_frame;
        file >> num_key_frame;
        for (int j = 0; j < num_key_frame; ++j) {
            // キーフレームの読み込み
            KeyFrame key_frame;
            file >> key_frame.frame_no;
            file >> key_frame.scale.x
                >> key_frame.scale.y
                >> key_frame.scale.z;
            file >> key_frame.rotation.x
                >> key_frame.rotation.y
                >> key_frame.rotation.z
                >> key_frame.rotation.w;
            file >> key_frame.position.x
                >> key_frame.position.y
                >> key_frame.position.z;
            bone_key_frames_[bone_name].push_back(key_frame);

            // 最終フレームの更新
            last_frame_ = std::max(last_frame_, key_frame.frame_no);
        }
    }
}

// 指定ボーンのキーフレームデータ取得
Animation::KeyFrame Animation::get_key_frame(
    const std::string& bone_name, float frame_no) const {
    // ボーン名でキーフレーム検索
    const auto& keys = bone_key_frames_.find(bone_name);
    KeyFrame result;
    if (keys == bone_key_frames_.end()) {
        // 指定ボーンのキーフレームが見つからない場合の例外処理
        result.frame_no = frame_no;
        result.scale = Vector3{ 1.0f, 1.0f, 1.0f };
        result.rotation = Quaternion{ 0.0f, 0.0f, 0.0f, 1.0f };
        result.position = Vector3{ 0.0f, 0.0f, 0.0f };
        return result;
    }
    // キーフレームの区間を検索
    const auto& segment = find_key_frame_segment(keys->second, frame_no);
    // 補間が必要なキーフレームが見つかるか？
    if (segment.first == segment.second) {
        return keys->second[segment.first];
    }
    // 開始キーフレーム取得
    const auto& start = keys->second[segment.first];
    // 終了キーフレーム取得
    const auto& end = keys->second[segment.second];
    // 補間値を求める
    const float t = (frame_no - start.frame_no) / (end.frame_no - start.frame_no);
    result.frame_no = frame_no;
    // 拡縮キー
    result.scale = Vector3::lerp(start.scale, end.scale, t);
    // 回転キー
    result.rotation = Quaternion::slerp(start.rotation, end.rotation, t);
    // 移動キー
    result.position = Vector3::lerp(start.position, end.position, t);
    return result;
}

// 終了フレーム数を返す
float Animation::last_frame() const {
    return last_frame_;
}

// 消去
void Animation::clear() {
    bone_key_frames_.clear();
    last_frame_ = 0.0f;
}

// キーフレームの区間を検索（二分探索）
std::pair<int, int> Animation::find_key_frame_segment(
    const std::vector<KeyFrame>& keys, float frame_no) const {
    // 先頭のキーフレーム以前のフレームを指定しているか？
    int start = 0;
    if (frame_no <= keys[start].frame_no) {
        return std::pair<int, int>{ start, start };
    }
    // 末尾のキーフレーム以降のフレームを指定しているか？
    int end = (int)keys.size() - 1;
    if (keys[end].frame_no <= frame_no) {
        return std::pair<int, int>{ end, end };
    }
    // キーフレームの区間を二分探索（バイナリサーチ）
    while ((start + 1) < end) {
        int mid = (start + end) / 2;
        if (keys[mid].frame_no == frame_no) {
            return std::pair<int, int>{ mid, mid };
        }
        else if (keys[mid].frame_no < frame_no) {
            start = mid;
        }
        else {
            end = mid;
        }
    }
    return std::pair<int, int>{ start, end };
}

// 変換行列を計算する
Matrix4x4 Animation::KeyFrame::matrix() const {
    return Matrix4x4::TRS(position, rotation, scale);
}

// キーフレームの線形補間
Animation::KeyFrame Animation::KeyFrame::lerp(const KeyFrame& a, const KeyFrame& b, float t) {
    Animation::KeyFrame result;
    result.frame_no = 0.0f;
    result.scale = Vector3::lerp(a.scale, b.scale, t);
    result.rotation = Quaternion::slerp(a.rotation, b.rotation, t);
    result.position = Vector3::lerp(a.position, b.position, t);
    return result;
}