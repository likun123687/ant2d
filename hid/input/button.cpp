
#include <hid/input/button.h>
#include <utils/debug.h>
namespace ant2d {

uint16_t Button::GetButtonId()
{
    return button_id_;
}

void Button::SetButtonId(uint16_t id)
{
    button_id_ = id;
}

bool Button::JustPressed()
{
    return (state_ & 0x02) == 2;
}

bool Button::JustReleased()
{
    return (state_ & 0x04) == 4;
}

bool Button::Down()
{
    return (state_ & 0x01) == 1;
}

void Button::Update(bool down)
{
    Info("btn update {}-{}", button_id_, down);
    auto d = ((state_ & 0x01) == 1);
    state_ &= (0x04 | 0x02);
    if (down) {
        state_ |= 0x01;
    }
    if (d && !down) {
        state_ |= 0x04;
    } else if (!d && down) {
        state_ |= 0x02;
    }
}

void Button::Reset()
{
    state_ &= 0x01;
}

ButtonManager::ButtonManager()
    : button_cache_ {}
{
}

Button* ButtonManager::NewButton()
{
    auto btn = &button_cache_[btn_id_];
    btn->SetButtonId(btn_id_);
    btn_id_++;
    return btn;
}
}
