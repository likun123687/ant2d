#pragma once
#include <gui/id.h>
#include <gui/gui_lib.h>
#include <gui/style.h>

namespace ant2d {
namespace layout {
    using LayoutType = int;
    constexpr LayoutType kVertical = 0;
    constexpr LayoutType kHorizontal = 1;
    constexpr LayoutType kOverLay = 2;

    using ViewType = int;
    constexpr ViewType kNormal = 0;
    constexpr ViewType kPopup = 0;

    struct Margin {
        float top;
        float left;
        float bottom;
        float right;
    };

    // GUI coordinate system
    // (0, 0)
    //  +-------------------------+ (w, 0)
    //  |  (x,y)                  |
    //  |   +------> W            |
    //  |   |                     |
    //  |   |                     |
    //  | H V                     |
    //  +-------------------------+
    //(0, h)                       (w, h)
    // Options is our layout-system
    struct Element {
        Element() = default;
        Element(gui::ID id_)
            : rect {}
            , margin {}
        {
            id = id_;
        }
        gui::ID id;
        gui::Rect rect;
        Margin margin;
    };

    struct FVec2 {
        float x;
        float y;
    };

    using DirtyFlag = uint32_t;

    constexpr DirtyFlag kFlagSize = 1 << 0;
    constexpr DirtyFlag kFlagMargin = 1 << 1;
    constexpr DirtyFlag kFlagGravity = 1 << 2;

    // Shadow of current ui-element
    class Options {
    public:
        gui::Rect rect_;
        Margin margin_;
        FVec2 gravity_;
        DirtyFlag flag_; // dirty flag
    public:
        // set flag
        Options* SetMargin(float top, float left, float right, float bottom);
        Options* SetSize(float w, float h);
        Options* SetGravity(float x, float y);
    };

    // Q. 当前 Group 的 X，Y, W, H 应该和 Group 的Cursor区分开来
    using Flag = uint32_t;
    class Group {
    public:
        LayoutType layout_type_;
        Flag flag_;
        Element* element_;

        // 仅用来缓存...
        FVec2 cursor_;
        FVec2 offset_;
        gui::Padding padding_;

        // 当前帧布局的计算变量
        struct {
            float w;
            float h;
        } size_;
        FVec2 gravity_;

        // true if group has a predefined size
        float fixed_width_;
        float fixed_height_;

    public:
        Group(LayoutType layout_type, Element* element);
        void SetGravity(float x, float y);
        void SetPadding(float top, float left, float right, float bottom);
        void SetSize(float w, float h);
    };

    class Layout {
    public:
        struct
            FVec2 cursor_;
        // ui bound 是一直存储的，记录一些持久化的数据
        std::vector<Element> ui_elements_;

        // group 是 fifo 的结构,记录动态的数据
        std::vector<Group> group_stack_;

        // header of group stack
        Group* h_group_;

        // default ui-element spacing
        float spacing_;

    public:
        Layout(gui::Theme* style);

        void SetDefaultLayoutSize(float w, float h);

        // 创建新的Layout
        Element* NewElement(gui::ID id);

        // 找出前一帧保存的大小
        std::tuple<Element*, bool> GetElement(gui::ID id);

        void Reset();

        // Options Operation
        Layout* Move(float x, float y);
        Element* BoundOf(gui::ID id);

        Layout* Offset(float dx, float dy);

        Layout* SetGravity(float x, float y);

        Layout* SetSize(float w, float h);

        Layout* SetPadding(float top, float left, float right, float bottom);

        std::tuple<Element*, bool> BeginLayout(gui::ID id, Options* opt, LayoutType xtype);

        // PopLayout, resume parent's state
        void EndLayout();

        std::tuple<Element*, bool> BeginElement(gui::ID id, Options* opt);

        void EndElement(Element* elem);

        // 重新计算父容器的大小
        // size + margin = BoundingBox
        void Extend(Element* elem);

        // 重新计算父容器的光标位置
        void Advance(Element* elem);
    };

} // namespace layout
} // namespace ant2d
