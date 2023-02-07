#pragma once
#include <gui/layout/layout_manager.h>

namespace ant2d {
namespace layout {

    LayoutManager::LayoutManager()
        : context_ { gui::Contex::DefaultContext() }
        , fallback_ { context_.GetTheme() }
        , layouts_ {}
    {
        current_ = &fallback_;
    }

    Element* LayoutManager::Text(gui::ID id, const std::string& text, gui::TextStyle* style, Options* opt)
    {
        auto [elem, ready] = BeginElement(id, opt);
        math::Vec2 size { 0, 0 };
        auto font = style->font;

        if (font == nullptr) {
            font = context_.GetTheme()->font;
        }

        // draw text 最好返回最新的大小..
        if (ready) {
            size = context_.DrawText(&elem->rect, text, style);
        } else {
            size = context_.CalcTextSize(text, 0, font, style->size);
        }

        elem->rect.w = size[0];
        elem->rect.h = size[1];

        EndElement(elem);
        return nullptr;
    }

    // Widgets: InputEditor
    void LayoutManager::InputText(const std::string& hint, Layout lyt, gui::InputStyle* style)
    {
    }

    // Widget: Image
    void LayoutManager::Image(gui::ID id, ITexture2D* tex, gui::ImageStyle* style, Options* opt)
    {
        auto [elem, ready] = BeginElement(id, opt);
        if (ready) {
            context_.DrawImage(&elem->rect, tex, style);
        } else {
            if (opt) {
                elem->rect.w = opt->rect_.w;
                elem->rect.h = opt->rect_.h;
            }
        }

        EndElement(elem);
    }

    // Widget: Button
    gui::EventType LayoutManager::Button(gui::ID id, const std::string& text, gui::ButtonStyle* style, Options* opt)
    {
        gui::EventType event = gui::Event::kEventNone;
        auto [elem, ready] = BeginElement(id, opt);
        if (ready) {
            event = context_.Button(id, &elem->rect, text, style);
        } else {
            auto font = style->text_style.font;
            if (font == nullptr) {
                font = context_.GetTheme()->font;
            }

            auto size = style->text_style.size;
            auto text_size = context_.CalcTextSize(text, 0, font, size);
            auto ext_w = style->padding.left + style->padding.right;
            auto ext_h = style->padding.top + style->padding.bottom;
            elem->rect.w = text_size[0] + ext_w;
            elem->rect.h = text_size[1] + ext_h;
        }

        EndElement(elem);
        return event;
    }

    void LayoutManager::RenderTextClipped(const std::string& text, gui::Rect* bb, gui::TextStyle* style)
    {
        auto [x, y] = SharedScreen.Gui2Game(bb->x, bb->y);
        auto font = context_.GetTheme()->text.font;
        if (bb->w == 0) {
            context_.GetDrawList().AddText(math::Vec2 { x, y }, text, font, 12, { 0xFF, 0x00, 0x00, 0x00 }, 0);
        } else {
            context_.GetDrawList().AddText(math::Vec2 { x, y }, text, font, 12, { 0xFF, 0x00, 0x00, 0x00 }, bb->w);
        }
    }

    gui::EventType LayoutManager::ImageButton(gui::ID id, ITexture2D* normal, ITexture2D* pressed, gui::ImageButtonStyle* style, Options* opt)
    {
        auto [elem, ready] = BeginElement(id, opt);
        auto bb = &elem->rect;
        gui::EventType event = gui::Event::kEventNone;

        if (ready) {
            event = context_.ClickEvent(id, bb);
            ITexture2D* tex = nullptr;
            if ((event & gui::Event::kEventDown) != 0) {
                tex = pressed;
            } else {
                tex = normal;
            }
            context_.DrawImage(bb, tex, &style->image_style);
        } else {
            if (opt) {
                elem->rect.w = opt->rect_.w;
                elem->rect.h = opt->rect_.h;
            }
        }

        EndElement(elem);
        return event;
    }

    // Slider 需要设定一些自定义的属性，目前没有想好如何实现，先把逻辑实现了
    // 用两种颜色来绘制
    gui::EventType LayoutManager::Slider(gui::ID id, float* value, gui::SliderStyle* style, Options* opt)
    {
        auto [elem, ready] = BeginElement(id, opt);
        auto bb = &elem->rect;
        gui::EventType e = gui::Event::kEventNone;

        if (ready) {
            // 说明滑动了，那么应该使用最新的值，而不是传入的值
            auto [v, event] = context_.CheckSlider(id, bb);
            if ((event & gui::Event::kEventDragging) != 0) {
                *value = v;
                e = event;
            }

            context_.DrawRect(bb, style->bar, 5);
            context_.DrawCircle(bb->x + bb->w * (*value), bb->y + bb->h / 2, 10, style->knob);
        } else {
            // 设置默认的宽高
            if (elem->rect.w == 0) {
                elem->rect.w = 120;
            }

            if (elem->rect.h == 0) {
                elem->rect.h = 10;
            }
        }

        EndElement(elem);
        return e;
    }

    void LayoutManager::DefineLayout(const std::string& name, ViewType xt)
    {
        auto iter = layouts_.find(name);
        if (iter != layouts_.end()) {
            current_ = iter->second.get();
        } else {
            std::unique_ptr<Layout> layout { new Layout { context_.GetTheme() } };
            current_ = layout.get();
            layouts_[name] = std::move(layout);
        }
    }

    void LayoutManager::Clear(std::initializer_list<std::string> names)
    {
        for (auto& name : names) {
            layouts_.erase(name);
        }
    }

    // 计算单个UI元素
    // 如果有大小则记录出偏移和Margin
    // 否则只返回元素
    std::tuple<Element*, bool> LayoutManager::BeginElement(gui::ID id, Options* opt)
    {
        return current_->BeginElement(id, opt);
    }

    // 结束绘制, 每绘制完一个元素都要偏移一下光标
    void LayoutManager::EndElement(Element* elem)
    {
        current_->EndElement(elem);
    }

    void LayoutManager::BeginLayout(gui::ID id, Options* opt, LayoutType xtype)
    {
        depth_++;
        auto [elem, ok] = current_->BeginLayout(id, opt, xtype);
        if (ok) {
            context_.DrawDebugBorder(elem->rect.x, elem->rect.y, elem->rect.w, elem->rect.h, {0xFF, 0x00, 0xFF, 0x00});
        }
    }

    void LayoutManager::EndLayout()
    {
        depth_--;
        current_->EndLayout();
        auto d = depth_;
        if (d == 0) {
            current_ = &fallback_;
        }
    }
} // namespace layout
} // namespace ant2d
