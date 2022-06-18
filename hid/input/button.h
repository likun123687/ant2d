
namespace hid
{

    // JustPressed/JustReleased 表示按键事件
    // IsDown 表示按键状态
    //
    // 0x0001 记录当前状态
    // 0x0002 记录pressed状态
    // 0x0004 记录release状态
    class Button
    {
        private:
            uint16_t state_;
            uint16_t button_id;

            uint16_t GetButtonId()
            {
                return button_id;
            }
            
            void SetButtonId(uint16_t id)
            {
                button_id = id;
            }
        bool JustPressed()
        {
            return (state_ & 0x02) == 2;
        }
        bool JustReleased()
        {
            return (state_ & 0x04) == 4;
        }

        bool Down()
        {
            return (state_ & 0x01) == 1;
        }
    
        void Update(bool down)
        {
            auto d = ((btn.state & 0x01) == 1);
            state_ &= (0x04|0x02);
            if (down) {
                state_ |= 0x01
            }
            if (d && !down) {
                state_ |= 0x04;
            } else if (!d && down) {
                state_ |= 0x02;
            }
        }

        void Reset()
        {
            state_ = state_ & 0x01;
        }
    };

    class ButtonManager
    {
        private:
        std::array<Button, 64> button_cache_;
        std::vector<uint16_t> frees_;

        uint16_t index_;
        public:
            Button *NewButton()
            {
                Button *btn = nullptr;
                if (!frees_.empty()) {
                    btn = &button_cache_[frees_.back()];
                    frees_.pop_back();
                } else {
                    btn = &button_cache_[index_];
                    btn->SetButtonId(index_);
                    index_++;
                }
                return btn;
            }

            void DeleteButton(Button *button)
            {
                auto id = button->GetButtonId();
                frees_.push_back(id);
                Button->Reset();
            }
    };
}
