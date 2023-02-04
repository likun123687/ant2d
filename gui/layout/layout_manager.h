#pragma once
#include <gui/gui_lib.h>
#include <gui/layout/layout.h>
#include <gui/screen_size.h>

namespace ant2d {
namespace layout {
    class LayoutManager {
    public:
        gui::Contex& context_;
        Options options_;
        Layout fallback_;
        std::unordered_map<std::string, std::unique_ptr<Layout>> layouts_;
        int depth_;
        Layout* current_;

        // sqNum should be same for  layout and drawing
        int sq_num_;

        LayoutManager();

        Element* Text(gui::ID id, const std::string& text, gui::TextStyle* style, Options* opt);

        // Widgets: InputEditor
        void InputText(const std::string& hint, Layout lyt, gui::InputStyle* style);

        // Widget: Image
        void Image(gui::ID id, ITexture2D* tex, gui::ImageStyle* style, Options* opt);

        // Widget: Button
        gui::EventType Button(gui::ID id, const std::string& text, gui::ButtonStyle* style, Options* opt);

        void RenderTextClipped(const std::string& text, gui::Rect* bb, gui::TextStyle* style);
        gui::EventType ImageButton(gui::ID id, ITexture2D* normal, ITexture2D* pressed, gui::ImageButtonStyle* style, Options* opt);

        // Slider 需要设定一些自定义的属性，目前没有想好如何实现，先把逻辑实现了
        // 用两种颜色来绘制
        gui::EventType Slider(gui::ID id, float* value, gui::SliderStyle* style, Options* opt);

        void DefineLayout(const std::string& name, ViewType xt);
        void Clear(std::initializer_list<std::string> names);

        // 计算单个UI元素
        // 如果有大小则记录出偏移和Margin
        // 否则只返回元素
        std::tuple<Element*, bool> BeginElement(gui::ID id, Options* opt);
        // 结束绘制, 每绘制完一个元素都要偏移一下光标
        void EndElement(Element* elem);
        void BeginLayout(gui::ID id, Options* opt, LayoutType xtype);
        void EndLayout();

        static LayoutManager& GLayoutManager()
        {
            static LayoutManager manager {};
            return manager;
        }
    };
} // namespace layout
} // namespace ant2d
