#pragma once
#include <gfx/i_comp.h>
#include <gfx/mesh/mesh.h>
#include <gfx/render_data.h>

namespace ant2d {
class MeshComp : public IComp {
private:
    Mesh mesh_;
    ZOrder z_order_;
    math::Vec2 size_;
    bool visible_;

public:
    std::tuple<float, float> Size()
    {
        return std::make_tuple(size_[0], size_[1]);
    }

    void SetSize(float width, float height)
    {
        size_[0] = width;
        size_[1] = height;
    }

    void SetVisible(bool v)
    {
        visible_ = v;
    }

    bool GetVisible()
    {
        return visible_;
    }

    Mesh* GetMesh()
    {
        return &mesh_;
    }

    ZOrder GetZOrder()
    {
        return z_order_;
    }

    void Reset()
    {
    }
};
}