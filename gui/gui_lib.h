#pragma once
#include <math/vector.h>
#include <gui/id.h>
#include <gui/style.h>
#include <gui/screen_size.h>
#include <hid/input/input_system.h>
#include <gui/drawing.h>
#include <gfx/bk_texture.h>
#include <utils/singleton.h>

namespace ant2d {
namespace gui {
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
        Contex(Theme* style);

        void Text(ID id, Rect* bb, const std::string& text, TextStyle* style);

        // Widgets: InputEditor
        void InputText(ID id, Rect* bb, const std::string& hint, InputStyle* style);

        // Widget: Image
        void Image(ID id, Rect* bb, ITexture2D* tex, ImageStyle* style);
        // Widget: Button
        EventType Button(ID id, Rect* bb, const std::string& text, ButtonStyle* style);
        void ImageBackground(uint8_t event_type);
        uint8_t ImageButton(ID id, ITexture2D* normal, ITexture2D* pressed, Rect* bb, ImageButtonStyle* style);
        EventType Slider(ID id, Rect* bb, float* value, SliderStyle* style);

        // Scroll 效果的关键是使用裁切限制滚动区域，然后
        // 通过计算拖拽，来得到争取的偏移
        void StartScroll(math::Vec2 size, math::Vec2 offset);
        void EndScroll();
        std::tuple<float, EventType> CheckSlider(ID id, Rect* bound);

        EventType ClickEvent(ID id, Rect* rect);

        EventType DraggingEvent(ID id, Rect* bound);

        void DrawQuad(std::array<math::Vec2, 4>& vertex, uint32_t fill);
        void DrawGradient(Rect bb, uint32_t c0, uint32_t c1, bool vertical);

        void DrawRect(Rect* bb, uint32_t fill, float round);

        void DrawBorder(Rect* bb, uint32_t color, float round, float thick);

        void DrawDebugBorder(float x, float y, float w, float h, uint32_t color);

        // default segment = 12 TODO, circle scale factor
        void DrawCircle(float x, float y, float radius, uint32_t fill);

        // segment default=12
        void DrawCircleNoneFill(float x, float y, float radius, uint32_t stroke_color, int segment, float thickness);
        void DrawImage(Rect* bound, ITexture2D* tex, ImageStyle* style);

        // 绘制元素, bb 存储相对于父容器的相对坐标..
        math::Vec2 DrawText(Rect* bb, const std::string& text, TextStyle* style);

        math::Vec2 CalcTextSize(const std::string& text, float wrap_width, font::FontAtlas* fnt, float font_size);
        // 偷师 flat-ui 中的设计，把空间的前景和背景分离，背景单独根据事件来变化..
        // 在 Android 中，Widget的前景和背景都可以根据控件状态发生变化
        // 但是在大部分UI中，比如 Text/Image 只会改变背景的状态
        // 偷懒的自定义UI，不做任何状态的改变... 所以说呢, 我们也采用偷懒的做法呗。。
        void ColorBackground(EventType event, Rect* bb, uint32_t normal, uint32_t pressed, float round);

        // Clip:
        void PushClipRect(math::Vec2 min_clip, math::Vec2 max_clip, bool intersect_current);

        // Theme:
        void UseTheme(Theme* style);
        Theme* GetTheme()
        {
            return theme_;
        }

        Cursor& GetCursor()
        {
            return cursor_;
        }

        DrawList& GetDrawList()
        {
            return draw_list_;
        }

        static Contex& DefaultContext()
        {
            static Contex contex { &Theme::LightTheme() };
            return contex;
        }
    };

}
} // namespace ant2d
