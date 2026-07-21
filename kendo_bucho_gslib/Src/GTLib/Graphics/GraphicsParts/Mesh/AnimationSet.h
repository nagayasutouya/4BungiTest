#ifndef ANIMATION_SET_H_
#define ANIMATION_SET_H_

#include <vector>
#include <string>
#include "Animation.h"

// アニメーションセットクラス
class AnimationSet {
public:
    // アニメーションファイル読み込み
    void load(const std::string& file_path);
    // 指定アニメーションを取得する
    const Animation& get(int anim_id) const;
    // アニメーションの数を取得する
    std::size_t count() const;
    // メモリ解放
    void clear();

private:
    std::vector<Animation> animations_;
};

#endif