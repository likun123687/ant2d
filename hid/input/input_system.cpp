
#include <hid/input/input_system.h>

namespace ant2d {

SparseMap::SparseMap()
    : keys_ {}
    , stat_ {}
    , used_ { 0 }
{
}

void SparseMap::PutKey(Key k, bool st)
{
    keys_[used_] = k;
    stat_[used_] = st;
    used_++;
}

void SparseMap::Clear()
{
    used_ = 0;
}

std::tuple<bool, bool> SparseMap::GetKey(Key k)
{
    for (int i = 0; i < used_; i++) {
        if (keys_[i] == k) {
            return { stat_[i], true };
        }
    }
    return { false, false };
}

int SparseMap::GetUsed()
{
    return used_;
}

InputSystem::InputSystem()
    : buttons_ {}
    , dirty_ {}
    , binds_ {}
    , active_ { 0 }
    , pointer_button_ {}
    , pointers_ {}
    , button_manager_ {}
{
}

/// 查询虚拟按键的状态
Button* InputSystem::GetButton(const std::string& name)
{
    return buttons_[name];
}

bool InputSystem::AnyKeyChanged()
{
    if (dirty_.GetUsed() > 0) {
        return true;
    }
    return false;
}

/// 将物理按键映射到虚拟按键
void InputSystem::RegisterButton(const std::string& name, std::initializer_list<Key> keys)
{
    auto btn = button_manager_.NewButton();
    buttons_[name] = btn;
    for (auto k : keys) {
        binds_.push_back({ k, btn });
    }
}

Button* InputSystem::PointerButton(KeyPoint pb)
{
    return &pointer_button_[static_cast<int>(pb)];
}

PointerInput InputSystem::PointerPosition(KeyPoint pb)
{
    return pointers_[static_cast<int>(pb)];
}

// Touch event
std::tuple<Button*, math::Vec2, math::Vec2> InputSystem::Touch(FingerId fi)
{
    Button* btn = &pointer_button_[fi];
    auto& p = pointers_[fi];
    auto pos = p.mouse_pos;
    auto delta = p.mouse_delta;
    return { btn, pos, delta };
}

// Mouse event
std::tuple<Button*, math::Vec2, math::Vec2> InputSystem::Mouse(int key)
{
    Button* btn = &pointer_button_[key];
    auto& p = pointers_[key];
    auto pos = p.mouse_pos;
    auto delta = p.mouse_delta;
    return { btn, pos, delta };
}

// 更新 Button 状态....
// TODO 此处的输入状态，更新有bug！！
void InputSystem::AdvanceFrame()
{
    auto n = binds_.size();
    auto dirty = dirty_.GetUsed();
    if (n > 0) {
        bool ok = false;
        bool st = false;
        Button* pr = nullptr;

        for (auto& bd : binds_) {
            auto [s, o] = dirty_.GetKey(bd.key);
            if (o) {
                st = st || s;
                ok = ok || o;
            }
            if (pr != bd.btn) {
                if (ok) {
                    bd.btn->Update(st);
                }

                st = false;
                ok = false;
            }
            pr = bd.btn;
        }
    }
}

void InputSystem::Reset()
{
    dirty_.Clear();
    // reset button state
    for (auto& kv : buttons_) {
        kv.second->Reset();
    }

    for (int i = 0; i < active_; i++) {
        pointer_button_[i].Reset();
    }
}

// 更新 key 的状态
void InputSystem::SetKeyEvent(int key, bool pressed)
{
    dirty_.PutKey(Key(key), pressed);
}

// 更新 Mouse/Touch 状态
void InputSystem::SetPointerEvent(int key, bool pressed, float x, float y)
{
    if (key != -1000) {
        pointer_button_[key].Update(pressed);
        pointers_[key].mouse_pos = math::Vec2 { x, y };

        if (key > active_) {
            active_ = key;
        }
    } else {
        // 如果是鼠标总是记录在 0 的位置
        // 如果是手指... 这就尴尬了..需要特殊处理
        pointers_[0].mouse_pos = math::Vec2 { x, y };
    }
}
}