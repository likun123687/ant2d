
#pragma once
#include <cstdint>
#include <array>
#include <vector>

namespace ant2d {

// JustPressed/JustReleased 表示按键事件
// IsDown 表示按键状态
//
// 0x0001 记录当前状态
// 0x0002 记录pressed状态
// 0x0004 记录release状态
class Button {
private:
    uint16_t state_ = 0;
    uint16_t button_id_ = 0;

public:
    uint16_t GetButtonId();
    void SetButtonId(uint16_t id);

    bool JustPressed();

    bool JustReleased();

    bool Down();

    void Update(bool down);

    void Reset();
};

class ButtonManager {
private:
    std::array<Button, 64> button_cache_;
    uint16_t btn_id_ = 0;

public:
    ButtonManager();
    Button* NewButton();
};
}
