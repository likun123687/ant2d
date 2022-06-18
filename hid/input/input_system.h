

using Key = int;
class KeyBind
{
    Key key_;
    Button *btn_;
};

class SparseMap
{
    private:
        std::array<Key, 6> keys_;
        std::array<bool, 6> states_;
        int used_;
    public:
        void Put(Key key, bool state)
        {
            keys_[used_] = key;
            states_[used_] = state;
            used_++;
        }

        void Clear()
        {
            used = 0;
        }

        std::tuple<bool bool> Get(Key key)
        {
            bool state = false;
            bool ok = false;

            for (int i = 0; i < used_; i++) {
                if (keys_[i] == key) {
                    state = states_[i];
                    ok = true;
                    break;
                }
            }

            return std::make_tuple(state, ok);
        }

}


class InputSystem
{
    std::unordered_map<std::string, Button*> buttons_;
    // 记录每帧的按键状态
    // 无论是用数组还是哈希，这里的实现总之要达到快速
    // 查询一个按键的状态的效果
    SparseMap dirty_;

    // 按照button排序，这样同一个Button的绑定按键是
    // 是连续的。
    std::vector<KeyBind> binds_;
    int active_;
    std::array<Button, 10> pointer_buttons_;
    std::array<PointerInput, 10> pointers_;

    /// 查询虚拟按键的状态
    Button *Button(const std::string& name)
    {
        return buttons_[name];
    }

    bool AnyKeyChanged()
    {
        return dirty_.GetUsed() > 0;
    }

    /// 将物理按键映射到虚拟按键
    void RegisterButton(const std::string &name, std::vector<Key> keys)
    {
        auto button = SharedButtonManager.NewButton();
        buttons_[name] = button;
        for (auto k:keys) {
            binds_.push_back(KeyBind{k, button});
        }
    }

    void AdvanceFrame()
    {
        auto size = binds_.size();
        auto dirty = dirty_.GetUsed();
        if (size > 0 && dirty > 0) {
            bool state = false;
            bool ok = false;
            Button *btn = nullptr;
            for (auto &bd:binds_) {
                auto [key_state, key_ok] = dirty_.Get(bd.key);
                if (ok) {
                    state = state || key_state;
                    ok = ok || key_ok;
                }

                if (btn != bd.btn) {
                    if (ok) {
                        bd.btn.Update(state);
                    }
                    state = false;
                    ok = false;
                }
                btn = bd.btn;
            }
        }
    }

    void Reset()
    {
        dirty_.clear();
        for (auto &b:buttons_) {
            SharedButtonManager.DeleteButton(b);
        }

        for (int i = 0; i< active_; i++) {
            pointer_buttons_[i].Reset();
        }
    }
    // 更新 key 的状态
    void SetKeyEvent(Key key, bool pressed)
    {
        dirty_.Put(key, pressed);
    }

    // 更新 Mouse/Touch 状态
    void SetPointerEvent(int key, bool pressed, float x, float y)
    {
        if (key != -1000) {
            pointer_buttons_[key].Update(pressed);
            pointers_[key].MousePos = math::Vec2{x, y};
            if (key > active_) {
                active_ = key;
            }
        } else {
            // 如果是鼠标总是记录在 0 的位置
            // 如果是手指... 这就尴尬了..需要特殊处理
            pointers_[0].MousePos = math::Vec2{x, y};
        }
    }

};

