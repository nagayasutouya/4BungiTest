#include "AnimationSet.h"
#include <fstream>
#include <stdexcept>

// アニメーションファイル読み込み
void AnimationSet::load(const std::string& file_path) {
    std::ifstream file{ file_path };
    if (!file) {
        throw std::runtime_error("can not open " + file_path);
    }
    // アニメーション数の読み込み
    int num_animations;
    file >> num_animations;
    animations_.resize(num_animations);
    for (Animation& animation : animations_) {
        // アニメーションデータの読み込み（ファイルストリームを渡す）
        animation.load(file);
    }
}

// 指定アニメーションを取得する
const Animation& AnimationSet::get(int anim_id) const {
    return animations_[anim_id];
}

// アニメーションの数を取得する
std::size_t AnimationSet::count() const {
    return animations_.size();
}

// メモリ解放
void AnimationSet::clear() {
    animations_.clear();
}