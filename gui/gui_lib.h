#include <math/vector.h>
#include <gui/id.h>
#include <gui/style.h>
#include <gui/screen_size.h>
#include <hid/input/input_system.h>

namespace ant2d {
using EventType = uint8_t;
struct Event {
    static constexpr EventType kEventNone = 0;
    static constexpr EventType kEventWentDown = 0x1 << 1;
    static constexpr EventType kEventWentUp = 0x1 << 2;
    static constexpr EventType kEventDown = 0x1 << 3;
    static constexpr EventType kEventStartDrag = 0x1 << 4;
    static constexpr EventType kEventEndDrag = 0x1 << 5;
    static constexpr EventType kEventDragging = 0x1 << 6;

    static bool JustPressed(EventType et)
    {
        return (et & kEventWentDown) != 0;
    }

    static bool JustReleased(EventType et)
    {
        return (et & kEventWentUp) != 0;
    }

    static bool Down(EventType et)
    {
        return (et & kEventDown) != 0;
    }

    static bool StartDrag(EventType et)
    {
        return (et & kEventStartDrag) != 0;
    }

    static bool EndDrag(EventType et)
    {
        return (et & kEventEndDrag) != 0;
    }

    static bool Dragging(EventType et)
    {
        return (et & kEventDragging) != 0;
    }
};

struct Rect {
public:
    float x;
    float y;
    float w;
    float h;

    Rect* Offset(float dx, float dy)
    {
        x = x + dx;
        y = y + dy;
        return this;
    }

    Rect* Scale(float skx, float sky)
    {
        x *= skx;
        y *= sky;
        w *= skx;
        h *= sky;
        return this;
    }

    bool InRange(math::Vec2 p)
    {
        if (x < p[0] && p[0] < (x + w)
            && y < p[1] && p[1] < (y + h)) {
            return true;
        }
        return false;
    }
};

struct Cursor {
    float x;
    float y;
};

// 一个Context维护UI层逻辑:
// 1. 一个 DrawList，负责生成顶点
// 2. 共享的 Theme，指针引用，多个 Context 之间可以共享
// 3. State 维护，负责维护当前的 UI 状态，比如动画，按钮，列表...
// 窗口在 Context 之上维护，默认的Context可以随意的绘制
// 在窗口内绘制的UI会受到窗口的管理.
class Contex {
private:
    struct State {
        ID hot;
        ID active;
        int mouse_x;
        int mouse_y;
        int mouse_down;

        // drag state
        ID dragging_pointer;
        math::Vec2 dragging_start;

        bool is_last_event_pointer_type;
        ID pointer_capture;
    };

    DrawList draw_list_;
    Theme* theme_;
    Cursor cursor_;
    State state_;

    // sqNum should be same for  layout and drawing
    int sq_num_;

public:
    Contex(Theme* style)
        : draw_list_ {}
    {
        theme_ = style;
        state_.dragging_pointer = -1;
        state_.is_last_event_pointer_type = false;
        state_.pointer_capture = -1;
    }

    void Text(ID id, Rect* bb, const std::string& text, TextStyle* style)
    {
        if (bb->w != 0) {
            DrawText(bb, text, style);
        } else {
            auto font = style->Font;
            if (font == nullptr) {
                font = theme_->Font;
            }

            auto sz = CalcTextSize(text, 0, font, style->size);
            bb->w = sz[0];
            bb->h = sz[1];
            DrawText(bb, text, style);
        }
    }

    // Widgets: InputEditor
    void InputText(ID id, Rect* bb, const std::string& hint, InputStyle* style)
    {
    }

    // Widget: Image
    void Image(ID id, Rect* bb, Texture2D* tex, ImageStyle* style)
    {
        DrawImage(bb, tex, style);
    }

    // Widget: Button
    EventType Button(ID id, Rect* bb, const std::string& text, ButtonStyle* style)
    {
        float round = theme_->button.rounding;
        math::Vec2 offset;
        font::FontAtlas* font = nullptr;

        if (style == nullptr) {
            style = theme_->button;
        }

        if (style->text_style.font != nullptr) {
            font = style->text_style.font;
        } else {
            font = theme_->font;
        }

        if (bb->w == 0) {
            auto text_size = CalcTextSize(text, 0, font, style->text_style.size);
            auto ext_w = style->padding.left + style->padding.right;
            auto ext_h = style->padding.top + style->padding.bottom;

            bb->w = text_size[0] + ext_w;
            bb->h = text_size[1] + ext_h;
        } else {
            // if button has size, gravity will effect text's position
            auto font = style->text_style.font;
            if (font == nullptr) {
                font = theme_->font;
            }
            auto text_size = CalcTextSize(text, 0, font, style->text_style.size);
            auto g = style->gravity;
            offset[0] = (bb->w - text_size[0] - style->padding.left - style->padding.right) * g[0];
            offset[1] = (bb->h - text_size[1] - style->padding.top - style->padding.bottom) * g[1];
        }

        // Check Event
        auto event = ClickEvent(id, bb);
        // Render Frame
        auto bg = style->back_ground;
        if (bg.normal != 0x0) {
            ColorBackground(event, bb, bg.normal, bg.pressed, round);
        } else {
            ColorBackground(event, bb, ThemeLight.Normal, ThemeLight.Pressed, round);
        }

        // Render Text
        bb->x += offset[0] + style->padding.left;
        bb->y += offset[1] + style->padding.top;

        DrawText(bb, text, &style->text_style);
        return event;
    }

    void ImageBackground(uint8_t event_type)
    {
    }

    uint8_t ImageButton(ID id, Texture2D* normal, Texture2D* pressed, Rect* bb, ImageButtonStyle* style)
    {
        if (style == nullptr) {
            style = theme_->image_button;
        }

        uint8_t event = ClickEvent(id, bb);
        Texture2D* tex = nullptr;
        if (event.Down()) {
            tex = pressed;
        } else {
            tex = normal;
        }
        DrawImage(bb, tex, style->image_style);
        return event;
    }

    EventType Slider(ID id, Rect* bb, float* value, SliderStyle* style)
    {
        EventType e;
        if (style == nullptr) {
            style = theme_->slider;
        }

        auto [v, event] = CheckSlider(id, bb);
        // 说明滑动了，那么应该使用最新的值，而不是传入的值
        if (event & Event::kEventDragging != 0) {
            *value = v;
            e = event;
        }
        DrawRect(bb, style->bar, 5);
        DrawCircle(bb->x + bb->w * (*value), bb->y + bb->h / 2, 10, style->knob);
        return e;
    }

    // Scroll 效果的关键是使用裁切限制滚动区域，然后
    // 通过计算拖拽，来得到争取的偏移
    void StartScroll(math::Vec2 size, math::Vec2 offset)
    {
    }

    void EndScroll()
    {
    }

    std::tuple<float, EventType> CheckSlider(ID id, Rect* bound)
    {
        EventType event = DraggingEvent(id, bound);
        float v = 0;
        if ((event & Event::kEventStartDrag) != 0) {
            state_.pointer_capture = id;
        }
        if ((event & Event::kEventEndDrag) != 0) {
            state_.pointer_capture = -1;
        }
        // Update the knob position & default = Horizontal
        if ((event & (Event::kEventDragging | Event::kEventWentDown)) != 0) {
            auto p1 = (SharedInputSystem.PointerPosition(0).mouse_pos[0]) / SharedScreen.scale_x_;
            auto p0 = bound->x + cursor_.x;
            v = (p1 - p0) / bound->w;
            if (v > 1) {
                v = 1
            }
            if (v < 0) {
                v = 0
            }
        }
        return { event, v };
    }

    EventType ClickEvent(ID id, Rect* rect)
    {
        EventType event = Event::kEventNone;
        auto c = cursor_;
        auto bb = Rect { (c.x + rect->x) * SharedScreen.scale_x_,
            (c.x + rect->y) * SharedScreen.scale_y_,
            rect->w * SharedScreen.scale_x_,
            rect->h * SharedScreen.scale_y_ };

        auto p = SharedInputSystem.PointerPosition(0);
        if (bb.InRange(p.mouse_pos)) {
            auto btn = SharedInputSystem.PointerButton(0);
            if (btn->JustPressed()) {
                state_.active = id;
                event = Event::kEventWentDown;
            }

            if (btn->JustReleased() && state_.active == id) {
                event = Event::kEventWentUp;
                state_.active = -1;
            } else if (btn->Down() && state_.active == id) {
                event |= Event::kEventDown;
            }
        }
        return event;
    }

    EventType DraggingEvent(ID id, Rect* bound)
    {
        EventType event = Event::kEventNone;
        auto c = cursor_;
        auto bb = Rect {
            (c.x + bound->x) * SharedScreen.scale_x_,
            (c.y + bound->y) * SharedScreen.scale_y_,
            bound->w * SharedScreen.scale_x_,
            bound->h * SharedScreen.scale_y_
        };
        auto p = SharedInputSystem.PointerPosition(0);
        if (bb.InRange(p.mouse_pos) || state_.pointer_capture == id) {
            // in-dragging, The pointer is in drag operation
            auto btn = SharedInputSystem.PointerButton(0);
            if (state_.dragging_pointer == id && !btn->JustPressed()) {
                if (btn->JustReleased()) {
                    event = Event::kEventEndDrag;
                    state_.dragging_pointer = -1;
                    state_.dragging_start = math::Vec2 {};
                    event |= Event::kEventWentUp;
                } else if (btn->Down()) {
                    event = Event::kEventDragging;
                } else {
                    state_.dragging_pointer = -1;
                }
            } else {
                // Keep the click position, then use it to check a drag event
                if (btn->JustPressed()) {
                    state_.dragging_start = p.mouse_pos;
                }
                // If the next movement out of thresh-hold, then it's a drag event
                if (btn->Down() && bb.InRange(state_.dragging_start)) {
                    auto start_position = state_.dragging_start;
                    float drag_thresh_hold = 8;

                    auto bb = Rect {
                        start_position[0] - drag_thresh_hold,
                        start_position[1] - drag_thresh_hold,
                        drag_thresh_hold,
                        drag_thresh_hold
                    };

                    // Start drag event
                    if (!bb.InRange(p.mouse_pos)) {
                        event |= Event::kEventStartDrag;
                        state_.dragging_start = p.mouse_pos;
                        state_.dragging_pointer = id;
                    }
                }
            }
        }
        return event;
    }
};

} // namespace ant2d
