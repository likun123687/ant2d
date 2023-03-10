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
extern InputSystem* SharedInputSystem;
namespace gui {
    using EventType = uint8_t;
    struct Event {
        static constexpr EventType kEventNone = 0;
        static constexpr EventType kEventWentDown = 0x1 << 0;
        static constexpr EventType kEventWentUp = 0x1 << 1;
        static constexpr EventType kEventDown = 0x1 << 2;
        static constexpr EventType kEventStartDrag = 0x1 << 3;
        static constexpr EventType kEventEndDrag = 0x1 << 4;
        static constexpr EventType kEventDragging = 0x1 << 5;

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

    // ??????Context??????UI?????????:
    // 1. ?????? DrawList?????????????????????
    // 2. ????????? Theme???????????????????????? Context ??????????????????
    // 3. State ?????????????????????????????? UI ???????????????????????????????????????...
    // ????????? Context ????????????????????????Context?????????????????????
    // ?????????????????????UI????????????????????????.
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

        // Scroll ?????????????????????????????????????????????????????????
        // ?????????????????????????????????????????????
        void StartScroll(math::Vec2 size, math::Vec2 offset);
        void EndScroll();
        std::tuple<float, EventType> CheckSlider(ID id, Rect* bound);

        EventType ClickEvent(ID id, Rect* rect);

        EventType DraggingEvent(ID id, Rect* bound);

        void DrawQuad(std::array<math::Vec2, 4>& vertex, color::Byte4 fill);
        void DrawGradient(Rect bb, color::Byte4 c0, color::Byte4 c1, bool vertical);

        void DrawRect(Rect* bb, color::Byte4 fill, float round);

        void DrawBorder(Rect* bb, color::Byte4 color, float round, float thick);

        void DrawDebugBorder(float x, float y, float w, float h, color::Byte4 color);

        // default segment = 12 TODO, circle scale factor
        void DrawCircle(float x, float y, float radius, color::Byte4 fill);

        // segment default=12
        void DrawCircleNoneFill(float x, float y, float radius, color::Byte4 stroke_color, int segment, float thickness);
        void DrawImage(Rect* bound, ITexture2D* tex, ImageStyle* style);

        // ????????????, bb ???????????????????????????????????????..
        math::Vec2 DrawText(Rect* bb, const std::string& text, TextStyle* style);

        math::Vec2 CalcTextSize(const std::string& text, float wrap_width, font::FontAtlas* fnt, float font_size);
        // ?????? flat-ui ????????????????????????????????????????????????????????????????????????????????????..
        // ??? Android ??????Widget?????????????????????????????????????????????????????????
        // ??????????????????UI???????????? Text/Image ???????????????????????????
        // ??????????????????UI??????????????????????????????... ????????????, ???????????????????????????????????????
        void ColorBackground(EventType event, Rect* bb, color::Byte4 normal, color::Byte4 pressed, float round);

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
