#pragma once
#include <array>
#include <tuple>
#include <unordered_map>
#include <initializer_list>
#include <hid/input/button.h>
#include <hid/input/touch.h>
#include <math/vector.h>
#include <string>
#include <utils/singleton.h>

namespace ant2d {
using Key = int;
struct KeyBind {
    Key key;
    Button* btn;
};

using KeyPoint = int;
static constexpr KeyPoint KeyPoint1 = 0;
static constexpr KeyPoint KeyPoint2 = 1;
static constexpr KeyPoint KeyPoint3 = 2;
static constexpr KeyPoint KeyPoint4 = 3;
static constexpr KeyPoint KeyPoint5 = 4;
static constexpr KeyPoint KeyPoint6 = 5;
static constexpr KeyPoint KeyPoint7 = 6;
static constexpr KeyPoint KeyPoint8 = 7;
static constexpr KeyPoint KeyPoint9 = 8;
static constexpr KeyPoint KeyPointX = 9;

// 记录一帧之内的按键，一帧时间做多支持同时按6个按键
class SparseMap {
public:
    SparseMap();
    void PutKey(Key k, bool st);

    void Clear();

    std::tuple<bool, bool> GetKey(Key k);

    int GetUsed();

private:
    std::array<Key, 6> keys_;
    std::array<bool, 6> stat_;
    int used_ = 0;
};

class InputSystem {

public:
    InputSystem();

    /// 查询虚拟按键的状态
    Button* GetButton(const std::string& name);
    bool AnyKeyChanged();

    /// 将物理按键映射到虚拟按键
    void RegisterButton(const std::string& name, std::initializer_list<Key> keys);

    Button* PointerButton(KeyPoint pb);

    PointerInput PointerPosition(KeyPoint pb);
    // Touch event
    std::tuple<Button*, math::Vec2, math::Vec2> Touch(FingerId fi);

    // Mouse event
    std::tuple<Button*, math::Vec2, math::Vec2> Mouse(int key);

    // 更新 Button 状态....
    // TODO 此处的输入状态，更新有bug！！
    void AdvanceFrame();

    void Reset();

    // 更新 key 的状态
    void SetKeyEvent(int key, bool pressed);

    // 更新 Mouse/Touch 状态
    void SetPointerEvent(int key, bool pressed, float x, float y);

private:
    std::unordered_map<std::string, Button*> buttons_;
    // 记录每帧的按键状态
    // 无论是用数组还是哈希，这里的实现总之要达到快速
    // 查询一个按键的状态的效果
    SparseMap dirty_;

    // 按照button排序，这样同一个Button的绑定按键是
    // 是连续的。
    std::vector<KeyBind> binds_;

    // 触摸/鼠标, 最多支持10个手指头同时触摸
    // 通常情况下，active < 1
    int active_ = 0;
    std::array<Button, 10> pointer_button_;
    std::array<PointerInput, 10> pointers_;
    ButtonManager button_manager_;
};

}

// #define SharedInputSystem \
//     ant2d::Singleton<ant2d::InputSystem>()
