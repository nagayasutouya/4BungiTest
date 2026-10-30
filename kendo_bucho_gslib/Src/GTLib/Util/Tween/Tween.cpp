#include "Tween.h"
#include "ValueUnit.h"
#include "Vector3Unit.h"

std::list<TweenUnit*> Tween::units_;
// TweenUnitを追加して管理下に置く
void Tween::add(TweenUnit* unit) {
    units_.push_back(unit);
}
// 管理下のTweenUnitの更新処理を呼び出す
void Tween::update(float delta_time) {
    // 全てのTweenUnitの更新処理を呼び出す
    for (TweenUnit* unit : units_)unit->update(delta_time);
    // 終了したTweenUnitは削除する
    for (auto itr = units_.begin(); itr != units_.end(); ) {
        if ((*itr)->is_finished()) {
            // 中身のTweenUnitをdeleteしてから
            delete* itr;
            // リストの要素を削除する
            itr = units_.erase(itr);
        }
        else ++itr;
    }
}
// 管理下のTweenUnitを全て削除する
void Tween::clear() {
    for (TweenUnit* tween : units_)delete tween;
    units_.clear();
}
// float型の値をTweenする
TweenUnit& Tween::value(float from, float to, float duration, std::function<void(float)> update_func) {
    TweenUnit* unit = new ValueUnit(from, to, duration, update_func);
    add(unit); // リストに加えて管理対象とする
    return *unit; // TweenUnitの参照を返却する
}
// Vector3型の値をTweenする
TweenUnit& Tween::vector3(const Vector3& from, const Vector3& to, float duration, std::function<void(const Vector3&)> update_func) {
    TweenUnit* unit = new Vector3Unit(from, to, duration, update_func);
    add(unit);
    return *unit;
}
// 指定された時間後に処理を実行する
TweenUnit& Tween::delay_call(float delay_time, std::function<void()> callback) {
    TweenUnit* unit = new TweenUnit(delay_time);
    unit->on_complete(callback);
    add(unit);
    return *unit;
}
// 指定された名前のTweenをキャンセル（削除）する
void Tween::cancel(const std::string& name) {
    // 全ての項目を走査して、名前が一致したものを削除する
    for (auto itr = units_.begin(); itr != units_.end(); ) {
        if ((*itr)->name() == name) {
            delete* itr;
            itr = units_.erase(itr);
        }
        else ++itr;
    }
}