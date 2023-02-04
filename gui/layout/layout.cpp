#include <gui/layout/layout.h>

namespace ant2d {
namespace layout {
    Options* Options::SetMargin(float top, float left, float right, float bottom)
    {
        flag_ |= kFlagMargin;
        margin_.top = top;
        margin_.left = left;
        margin_.right = right;
        margin_.bottom = bottom;
        return this;
    }

    Options* Options::SetSize(float w, float h)
    {
        flag_ |= kFlagSize;
        rect_.w = w;
        rect_.h = h;
        return this;
    }

    Options* Options::SetGravity(float x, float y)
    {
        flag_ |= kFlagGravity;
        gravity_.x = x;
        gravity_.y = y;
        return this;
    }

    Group::Group(LayoutType layout_type, Element* element)
    {
        layout_type_ = layout_type;
        element_ = element;
    }

    void Group::SetGravity(float x, float y)
    {
        gravity_.x = math::Clamp(x, 0, 1);
        gravity_.y = math::Clamp(y, 0, 1);
    }

    void Group::SetPadding(float top, float left, float right, float bottom)
    {
        padding_.top = top;
        padding_.left = left;
        padding_.right = right;
        padding_.bottom = bottom;
    }

    void Group::SetSize(float w, float h)
    {
        fixed_width_ = w;
        fixed_height_ = h;
    }

    Layout::Layout(gui::Theme* style)
    {
        // init size, todo resize 会导致指针错误
        ui_elements_.reserve(32);
        group_stack_.reserve(8);
        spacing_ = style->spacing;

        // Create a default layout
        ui_elements_.emplace_back(0);
        group_stack_.emplace_back(kOverLay, &ui_elements_.back());
        h_group_ = &group_stack_.back();
    }

    void Layout::SetDefaultLayoutSize(float w, float h)
    {
        auto [dft, ok] = GetElement(0);
        if (ok) {
            dft->rect.w = w;
            dft->rect.h = h;
        }
    }

    // 创建新的Layout
    Element* Layout::NewElement(gui::ID id)
    {
        ui_elements_.emplace_back(id);
        return &ui_elements_.back();
    }

    // 找出前一帧保存的大小
    std::tuple<Element*, bool> Layout::GetElement(gui::ID id)
    {
        Element* bb = nullptr;
        bool ok = false;
        auto size = ui_elements_.size();
        if (size > int(id)) {
            bb = &ui_elements_[id];
            if (bb->id == id) {
                ok = true;
                return { bb, ok };
            }
        }

        // Linear Search
        for (int i = 0; i < size; i++) {
            bb = &ui_elements_[i];
            if (bb->id == id) {
                ok = true;
                break;
            }
        }
        return { bb, ok };
    }

    void Layout::Reset()
    {
        ui_elements_.resize(0);
    }

    // Options Operation
    Layout* Layout::Move(float x, float y)
    {
        cursor_.x = x;
        cursor_.y = y;
        return this;
    }

    Element* Layout::BoundOf(gui::ID id)
    {
        Element* bb = nullptr;
        int size = ui_elements_.size();
        if (size > int(id)) {
            bb = &ui_elements_[id];
            if (bb->id == id) {
                return bb;
            }
        }

        return bb;
    }

    Layout* Layout::Offset(float dx, float dy)
    {
        cursor_.x += dx;
        cursor_.y += dy;
        return this;
    }

    Layout* Layout::SetGravity(float x, float y)
    {
        h_group_->SetGravity(x, y);
        return this;
    }

    Layout* Layout::SetSize(float w, float h)
    {
        h_group_->SetSize(w, h);
        return this;
    }

    Layout* Layout::SetPadding(float top, float left, float right, float bottom)
    {
        h_group_->SetPadding(top, left, right, bottom);
        return this;
    }

    std::tuple<Element*, bool> Layout::BeginLayout(gui::ID id, Options* opt, LayoutType xtype)
    {
        // layout element
        auto spacing = spacing_;
        spacing_ = 0;
        auto [elem, ok] = BeginElement(id, opt);
        spacing_ = spacing;

        // do layout
        //  group-stack has a default parent
        //  so it's safe to index
        auto parent = &group_stack_.back();
        group_stack_.push_back({ xtype, elem });
        h_group_ = &group_stack_.back();

        // stash Options state
        parent->cursor_.x = cursor_.x;
        parent->cursor_.y = cursor_.y;

        cursor_.x = 0;
        cursor_.y = 0;
        return { elem, ok };
    }

    // PopLayout, resume parent's state
    void Layout::EndLayout()
    {
        // 1. Set size with external spacing
        auto v = h_group_;
        if (v->fixed_width_ > 0) {
            v->element_->rect.w = v->fixed_width_;
        } else {
            v->element_->rect.w = v->size_.w + spacing_;
        }

        if (v->fixed_height_ > 0) {
            v->element_->rect.h = v->fixed_height_;
        } else {
            v->element_->rect.h = v->size_.h + spacing_;
        }

        // 2. return to parent
        auto size = group_stack_.size();
        if (size > 1) {
            group_stack_.pop_back();
            h_group_ = &group_stack_.back();
        }

        auto g = h_group_;
        cursor_.x = g->cursor_.x;
        cursor_.y = g->cursor_.y;

        // 3. end layout
        auto elem = Element {};
        elem.rect.x = 0;
        elem.rect.y = 0;
        elem.rect.w = v->element_->rect.w;
        elem.rect.h = v->element_->rect.h;
        auto spacing = spacing_;
        spacing_ = 0;
        EndElement(&elem);
        spacing_ = spacing;
    }

    std::tuple<Element*, bool> Layout::BeginElement(gui::ID id, Options* opt)
    {
        auto [elem, ok] = GetElement(id);
        if (!ok) {
            elem = NewElement(id);
        } else {
            // 计算偏移
            elem->rect.x = cursor_.x + spacing_;
            elem->rect.y = cursor_.y + spacing_;

            // gravity
            auto group = h_group_;
            auto gravity = group->gravity_;
            struct
            {
                float w;
                float h;
            } extra { spacing_ * 2, spacing_ * 2 };

            // Each element's property
            if ((opt && opt->flag_) != 0) {
                // 计算 margin 和 偏移
                if ((opt->flag_ & kFlagMargin) != 0) {
                    elem->margin = opt->margin_;
                    elem->rect.x += elem->margin.left;
                    elem->rect.y += elem->margin.top;

                    extra.w += elem->margin.left + elem->margin.right;
                    extra.h += elem->margin.top + elem->margin.bottom;
                }
                // 计算大小
                if ((opt->flag_ & kFlagSize) != 0) {
                    if (opt->rect_.w > 0) {
                        elem->rect.w = opt->rect_.w;
                    }

                    if (opt->rect_.h > 0) {
                        elem->rect.h = opt->rect_.h;
                    }
                }

                // Overlap group's gravity
                if ((opt->flag_ & kFlagGravity) != 0) {
                    gravity = opt->gravity_;
                }

                // clear flag
                opt->flag_ = 0;
            }

            switch (group->layout_type_) {
            case kHorizontal:
                elem->rect.y += (group->element_->rect.h - elem->rect.h - extra.h) * gravity.y;
                break;
            case kVertical:
                elem->rect.x += (group->element_->rect.w - elem->rect.w - extra.w) * gravity.x;
                break;
            case kOverLay:
                elem->rect.y += (group->element_->rect.h - elem->rect.h - extra.h) * gravity.y;
                elem->rect.x += (group->element_->rect.w - elem->rect.w - extra.w) * gravity.x;
                break;
            }

            elem->rect.x += h_group_->element_->rect.x;
            elem->rect.y += h_group_->element_->rect.y;
        }

        return { elem, ok };
    }

    void Layout::EndElement(Element* elem)
    {
        if (!elem) {
            Error("element should no be null");
        }

        Advance(elem);
        Extend(elem);
    }

    // 重新计算父容器的大小
    // size + margin = BoundingBox
    void Layout::Extend(Element* elem)
    {
        auto g = h_group_;
        auto dx = elem->rect.w + elem->margin.left + elem->margin.right + spacing_;
        auto dy = elem->rect.h + elem->margin.top + elem->margin.bottom + spacing_;

        switch (g->layout_type_) {
        case kHorizontal:
            // 水平加之，高度取最大
            g->size_.w += dx;
            g->size_.h = math::Max(g->size_.h, dy);
            break;

        case kVertical:
            // 高度加之，水平取最大
            g->size_.w += math::Max(g->size_.w, dx);
            g->size_.h = dy;
            break;
        case kOverLay:
            // 重叠, 取高或者宽的最大值
            g->size_.w = math::Max(g->size_.w, dx);
            g->size_.h = math::Max(g->size_.h, dy);
            break;
        }
    }

    // 重新计算父容器的光标位置
    void Layout::Advance(Element* elem)
    {
        auto g = h_group_;
        auto c = &cursor_;

        auto dx = elem->rect.w + elem->margin.left + elem->margin.right + spacing_;
        auto dy = elem->rect.h + elem->margin.top + elem->margin.bottom + spacing_;

        switch (g->layout_type_) {
        case kHorizontal:
            // 水平步进，前进一个控件宽度
            c->x += dx;
            break;

        case kVertical:
            // 垂直步进，前进一个控件高度
            c->y += dy;
            break;
        case kOverLay:
            // 保持原来的位置不变..
            break;
        }
    }

} // namespace layout
} // namespace ant2d
