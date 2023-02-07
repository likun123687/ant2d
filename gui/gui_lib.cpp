#include <gui/gui_lib.h>
namespace ant2d {
namespace gui {

    Contex::Contex(Theme* style)
        : draw_list_ {}
    {
        theme_ = style;
        state_.dragging_pointer = -1;
        state_.is_last_event_pointer_type = false;
        state_.pointer_capture = -1;
    }

    void Contex::Text(ID id, Rect* bb, const std::string& text, TextStyle* style)
    {
        if (bb->w != 0) {
            DrawText(bb, text, style);
        } else {
            auto font = style->font;
            if (font == nullptr) {
                font = theme_->font;
            }

            auto sz = CalcTextSize(text, 0, font, style->size);
            bb->w = sz[0];
            bb->h = sz[1];
            DrawText(bb, text, style);
        }
    }

    // Widgets: InputEditor
    void Contex::InputText(ID id, Rect* bb, const std::string& hint, InputStyle* style)
    {
    }

    // Widget: Image
    void Contex::Image(ID id, Rect* bb, ITexture2D* tex, ImageStyle* style)
    {
        DrawImage(bb, tex, style);
    }

    // Widget: Button
    EventType Contex::Button(ID id, Rect* bb, const std::string& text, ButtonStyle* style)
    {
        float round = theme_->button.rounding;
        math::Vec2 offset { 0.0f, 0.0f };
        font::FontAtlas* font = nullptr;

        if (style == nullptr) {
            style = &(theme_->button);
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
        if (bg.normal.r != 0x0) {
            ColorBackground(event, bb, bg.normal, bg.pressed, round);
        } else {
            ColorBackground(event, bb, Theme::LightTheme().normal, Theme::LightTheme().pressed, round);
        }

        // Render Text
        bb->x += offset[0] + style->padding.left;
        bb->y += offset[1] + style->padding.top;

        DrawText(bb, text, &style->text_style);
        return event;
    }

    void Contex::ImageBackground(uint8_t event_type)
    {
    }

    uint8_t Contex::ImageButton(ID id, ITexture2D* normal, ITexture2D* pressed, Rect* bb, ImageButtonStyle* style)
    {
        if (style == nullptr) {
            style = &(theme_->image_button);
        }

        uint8_t event = ClickEvent(id, bb);
        ITexture2D* tex = nullptr;
        Info("event88 {:#x}", event);
        if (Event::Down(event)) {
            tex = pressed;
            // Info("event88 1111");
        } else {
            tex = normal;
            // Info("event88 2222");
        }
        DrawImage(bb, tex, &(style->image_style));
        return event;
    }

    EventType Contex::Slider(ID id, Rect* bb, float* value, SliderStyle* style)
    {
        EventType e;
        if (style == nullptr) {
            style = &(theme_->slider);
        }

        auto [v, event] = CheckSlider(id, bb);
        // 说明滑动了，那么应该使用最新的值，而不是传入的值
        if ((event & Event::kEventDragging) != 0) {
            Info("slider event {}", event);
            *value = v;
            e = event;
        }
        DrawRect(bb, style->bar, 5);
        DrawCircle(bb->x + bb->w * (*value), bb->y + bb->h / 2, 10, style->knob);
        return e;
    }

    // Scroll 效果的关键是使用裁切限制滚动区域，然后
    // 通过计算拖拽，来得到争取的偏移
    void Contex::StartScroll(math::Vec2 size, math::Vec2 offset)
    {
    }

    void Contex::EndScroll()
    {
    }

    std::tuple<float, EventType> Contex::CheckSlider(ID id, Rect* bound)
    {
        EventType event = DraggingEvent(id, bound);
        float v = 0;
        if ((event & Event::kEventStartDrag) != 0) {
            state_.pointer_capture = id;
            Info("checkslider 1111 {:#x}", event);
        }
        if ((event & Event::kEventEndDrag) != 0) {
            state_.pointer_capture = -1;
            Info("checkslider 2222 {:#x}", event);
        }
        // Update the knob position & default = Horizontal
        if ((event & (Event::kEventDragging | Event::kEventWentDown)) != 0) {
            auto p1 = (SharedInputSystem->PointerPosition(0).mouse_pos[0]) / SharedScreen.scale_x_;
            auto p0 = bound->x + cursor_.x;
            v = (p1 - p0) / bound->w;
            if (v > 1) {
                v = 1;
            }
            if (v < 0) {
                v = 0;
            }
            Info("checkslider 3333 {:#x} {} {}", event, v, SharedInputSystem->PointerPosition(0).mouse_pos[0]);
        }
        return { v, event };
    }

    EventType Contex::ClickEvent(ID id, Rect* rect)
    {
        EventType event = Event::kEventNone;
        auto& c = cursor_;
        Rect bb { (c.x + rect->x) * SharedScreen.scale_x_,
            (c.y + rect->y) * SharedScreen.scale_y_,
            rect->w * SharedScreen.scale_x_,
            rect->h * SharedScreen.scale_y_ };
        auto p = SharedInputSystem->PointerPosition(0);
        if (bb.InRange(p.mouse_pos)) {
            auto btn = SharedInputSystem->PointerButton(0);
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

    EventType Contex::DraggingEvent(ID id, Rect* bound)
    {
        EventType event = Event::kEventNone;
        auto& c = cursor_;
        Rect bb {
            (c.x + bound->x) * SharedScreen.scale_x_,
            (c.y + bound->y) * SharedScreen.scale_y_,
            bound->w * SharedScreen.scale_x_,
            bound->h * SharedScreen.scale_y_
        };
        auto p = SharedInputSystem->PointerPosition(0);
        if (bb.InRange(p.mouse_pos) || state_.pointer_capture == id) {
            // in-dragging, The pointer is in drag operation
            auto btn = SharedInputSystem->PointerButton(0);
            if (state_.dragging_pointer == id && !btn->JustPressed()) {
                if (btn->JustReleased()) {
                    event = Event::kEventEndDrag;
                    state_.dragging_pointer = -1;
                    state_.dragging_start = math::Vec2 { 0, 0 };
                    event |= Event::kEventWentUp;
                    Info("dragging event 1111 {:#x}", event);
                } else if (btn->Down()) {
                    event = Event::kEventDragging;
                    Info("dragging event 2222 {:#x}", event);
                } else {
                    state_.dragging_pointer = -1;
                    Info("dragging event 3333 {:#x}", event);
                }
            } else {
                // Keep the click position, then use it to check a drag event
                if (btn->JustPressed()) {
                    state_.dragging_start = p.mouse_pos;
                    Info("dragging event 4444 {:#x}", event);
                }
                // If the next movement out of thresh-hold, then it's a drag event
                if (btn->Down() && bb.InRange(state_.dragging_start)) {
                    auto start_position = state_.dragging_start;
                    float drag_thresh_hold = 8;

                    Rect bb {
                        start_position[0] - drag_thresh_hold,
                        start_position[1] - drag_thresh_hold,
                        drag_thresh_hold,
                        drag_thresh_hold
                    };

                    // Start drag event
                    if (!bb.InRange(p.mouse_pos)) {
                        Info("dragging event 5555 {:#x}", event);
                        event |= Event::kEventStartDrag;
                        state_.dragging_start = p.mouse_pos;
                        state_.dragging_pointer = id;
                    }
                }
            }
        }
        return event;
    }

    void Contex::DrawQuad(std::array<math::Vec2, 4>& vertex, color::Byte4 fill)
    {
        float cx = cursor_.x;
        float cy = cursor_.y;

        for (int i = 0; i < 4; i++) {
            std::tie(vertex[i][0], vertex[i][1]) = SharedScreen.Gui2Game(vertex[i][0] + cx, vertex[i][1] + cy);
        }
        draw_list_.AddQuadFilled(vertex[0], vertex[1], vertex[2], vertex[3], fill);
    }

    void Contex::DrawGradient(Rect bb, color::Byte4 c0, color::Byte4 c1, bool vertical)
    {
        float x = bb.x + cursor_.x;
        float y = bb.y + cursor_.y;

        std::tie(x, y) = SharedScreen.Gui2Game(x, y);

        auto min = math::Vec2 { x * SharedScreen.scale_x_, (y - bb.h) * SharedScreen.scale_y_ };
        auto max = math::Vec2 { (x + bb.w) * SharedScreen.scale_x_, y * SharedScreen.scale_y_ };

        auto dl = &draw_list_;
        dl->PrimReserve(6, 4);
        auto uv = dl->tex_uv_white_pixel_;
        auto a = min;
        auto b = math::Vec2 { max[0], min[1] };
        auto c = max;
        auto d = math::Vec2 { min[0], max[1] };
        if (vertical) {
            auto top = c0;
            auto down = c1;
            dl->vtx_writer_[0] = DrawVert { a, uv, down };
            dl->vtx_writer_[1] = DrawVert { b, uv, down };
            dl->vtx_writer_[2] = DrawVert { c, uv, top };
            dl->vtx_writer_[3] = DrawVert { d, uv, top };
        } else {
            auto left = c0;
            auto right = c1;
            dl->vtx_writer_[0] = DrawVert { a, uv, left };
            dl->vtx_writer_[1] = DrawVert { b, uv, right };
            dl->vtx_writer_[2] = DrawVert { c, uv, right };
            dl->vtx_writer_[3] = DrawVert { d, uv, left };
        }

        dl->idx_writer_[0] = DrawIdx(dl->vtx_index_ + 0);
        dl->idx_writer_[1] = DrawIdx(dl->vtx_index_ + 1);
        dl->idx_writer_[2] = DrawIdx(dl->vtx_index_ + 2);
        dl->idx_writer_[3] = DrawIdx(dl->vtx_index_ + 0);
        dl->idx_writer_[4] = DrawIdx(dl->vtx_index_ + 2);
        dl->idx_writer_[5] = DrawIdx(dl->vtx_index_ + 3);

        dl->vtx_index_ += 4;
        dl->idx_index_ += 6;
        dl->AddCommand(6);
    }

    void Contex::DrawRect(Rect* bb, color::Byte4 fill, float round)
    {
        auto x = bb->x + cursor_.x;
        auto y = bb->y + cursor_.y;
        std::tie(x, y) = SharedScreen.Gui2Game(x, y);
        auto min = math::Vec2 { x * SharedScreen.scale_x_, (y - bb->h) * SharedScreen.scale_y_ };
        auto max = math::Vec2 { (x + bb->w) * SharedScreen.scale_x_, y * SharedScreen.scale_y_ };

        draw_list_.AddRectFilled(min, max, fill, round * SharedScreen.scale_x_, FlagCornerAll);
    }

    void Contex::DrawBorder(Rect* bb, color::Byte4 color, float round, float thick)
    {
        auto x = bb->x + cursor_.x;
        auto y = bb->y + cursor_.y;
        std::tie(x, y) = SharedScreen.Gui2Game(x, y);
        auto min = math::Vec2 { x * SharedScreen.scale_x_, (y - bb->h) * SharedScreen.scale_y_ };
        auto max = math::Vec2 { (x + bb->w) * SharedScreen.scale_x_, y * SharedScreen.scale_y_ };

        draw_list_.AddRect(min, max, color, round * SharedScreen.scale_x_, FlagCornerAll, thick);
    }

    void Contex::DrawDebugBorder(float x, float y, float w, float h, color::Byte4 color)
    {
        x = x + cursor_.x;
        y = y + cursor_.y;
        auto min = math::Vec2 { x * SharedScreen.scale_x_, (y - h) * SharedScreen.scale_y_ };
        auto max = math::Vec2 { (x + w) * SharedScreen.scale_x_, y * SharedScreen.scale_y_ };

        draw_list_.AddRect(min, max, color, 0, FlagCornerNone, 1);
    }

    // default segment = 12 TODO, circle scale factor
    void Contex::DrawCircle(float x, float y, float radius, color::Byte4 fill)
    {
        std::tie(x, y) = SharedScreen.Gui2Game(x + cursor_.x, y + cursor_.y);
        x = x * SharedScreen.scale_x_;
        y = y * SharedScreen.scale_y_;
        draw_list_.AddCircleFilled(math::Vec2 { x, y }, radius * SharedScreen.scale_x_, fill, 12);
    }

    // segment default=12
    void Contex::DrawCircleNoneFill(float x, float y, float radius, color::Byte4 stroke_color, int segment, float thickness)
    {
        std::tie(x, y) = SharedScreen.Gui2Game(x + cursor_.x, y + cursor_.y);
        x = x * SharedScreen.scale_x_;
        y = y * SharedScreen.scale_y_;
        draw_list_.AddCircle(math::Vec2 { x, y }, radius * SharedScreen.scale_x_, stroke_color, segment, thickness);
    }

    void Contex::DrawImage(Rect* bound, ITexture2D* tex, ImageStyle* style)
    {
        auto min = math::Vec2 { bound->x + cursor_.x, bound->y + cursor_.y };
        if (bound->w == 0) {
            auto sz = tex->GetSize();
            bound->w = sz.width;
            bound->h = sz.height;
        }

        auto max = min.Add(math::Vec2 { bound->w, bound->h });
        color::Byte4 color { 0xFF, 0xFF, 0xFF, 0xFF };
        if (style != nullptr) {
            color = style->tint;
        } else {
            color = theme_->image.tint;
        }
        std::tie(min[0], min[1]) = SharedScreen.Gui2Game(min[0], min[1]);
        std::tie(max[0], max[1]) = SharedScreen.Gui2Game(max[0], max[1]);

        // scale
        min[0] = min[0] * SharedScreen.scale_x_;
        min[1] = min[1] * SharedScreen.scale_y_;
        max[0] = max[0] * SharedScreen.scale_x_;
        max[1] = max[1] * SharedScreen.scale_y_;

        auto rg = tex->GetRegion();
        if (rg.rotated) {
            draw_list_.AddImageQuad(tex->GetTextureId(),
                min, math::Vec2 { max[0], min[1] }, max, math::Vec2 { min[0], max[1] }, // xy
                math::Vec2 { rg.x2, rg.y1 }, math::Vec2 { rg.x2, rg.y2 },
                math::Vec2 { rg.x1, rg.y2 }, math::Vec2 { rg.x1, rg.y1 }, // uv
                color);
        } else {
            draw_list_.AddImage(tex->GetTextureId(), min, max,
                math::Vec2 { rg.x1, rg.y1 }, math::Vec2 { rg.x2, rg.y2 }, color);
        }
    }

    // 绘制元素, bb 存储相对于父容器的相对坐标..
    math::Vec2 Contex::DrawText(Rect* bb, const std::string& text, TextStyle* style)
    {
        auto [x, y] = SharedScreen.Gui2Game(bb->x + cursor_.x, bb->y + cursor_.y);
        Info("draw text  {}:{}:{}", text, x, y);
        auto font = style->font;
        if (font == nullptr) {
            font = theme_->font;
        }
        auto font_size = style->size * SharedScreen.scale_x_; // TODO 字体缩放不能这么简单的考虑
        auto color = style->color;
        auto wrap_width = (bb->w + 10) * SharedScreen.scale_x_;
        auto pos = math::Vec2 { x * SharedScreen.scale_x_, y * SharedScreen.scale_y_ };

        auto size = draw_list_.AddText(pos, text, font, font_size, color, wrap_width);
        return size;
    }

    math::Vec2 Contex::CalcTextSize(const std::string& text, float wrap_width, font::FontAtlas* fnt, float font_size)
    {
        return font::CalculateTextSize(text, fnt, font_size);
    }

    // 偷师 flat-ui 中的设计，把空间的前景和背景分离，背景单独根据事件来变化..
    // 在 Android 中，Widget的前景和背景都可以根据控件状态发生变化
    // 但是在大部分UI中，比如 Text/Image 只会改变背景的状态
    // 偷懒的自定义UI，不做任何状态的改变... 所以说呢, 我们也采用偷懒的做法呗。。
    void Contex::ColorBackground(EventType event, Rect* bb, color::Byte4 normal, color::Byte4 pressed, float round)
    {
        if ((event & Event::kEventDown) != 0) {
            DrawRect(bb, pressed, round);
        } else {
            DrawRect(bb, normal, round);
        }
    }

    // Clip:
    void Contex::PushClipRect(math::Vec2 min_clip, math::Vec2 max_clip, bool intersect_current)
    {
    }

    // Theme:
    void Contex::UseTheme(Theme* style)
    {
        theme_ = style;
    }

}
} // namespace ant2d
