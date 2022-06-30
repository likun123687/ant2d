#pragma once
#include <asset/shader_utils.h>
#include <vector>

namespace ant2d {

struct PosTexColorVertex {
    float x;
    float y;
    float u;
    float v;
    float r;
    float g;
    float b;
    float a;
    //uint32_t rgba;
};

class Camera;
class IRender {
protected:
    ShaderType shader_type_;

public:
    virtual void SetCamera(Camera* camera) = 0;
    ShaderType GetType()
    {
        return shader_type_;
    }
    void SetType(ShaderType type)
    {
        shader_type_ = type;
    }
    virtual ~IRender() = default;
};

struct Batch {
    uint16_t texture_id;
    int16_t depth;

    uint16_t vertex_id;
    uint16_t index_id;

    uint16_t first_vertex;
    uint16_t num_vertex;

    uint16_t first_index;
    uint16_t num_index;
};

class IBatchObject {
public:
    virtual void Fill(std::vector<PosTexColorVertex>& vertex, uint32_t vertex_pos) = 0;
    virtual int Size() = 0;
    virtual ~IBatchObject() = default;
};

struct SortObject {
    uint32_t sort_id;
    uint32_t value;
};

struct View {
    std::vector<SortObject> render_nodes;
    Camera* camera;
};

class RenderSystem;

class IRenderFeature {
public:
    virtual void Register(RenderSystem* rs) = 0;
    virtual void Extract(View* v) = 0;
    virtual void Draw(const std::vector<SortObject>& nodes) = 0;
    virtual void Flush() = 0;
    virtual ~IRenderFeature() = default;
};

class ZOrder {
private:
    uint16_t value_;

public:
    ZOrder():value_(0)
    {
    }
    void SetValue(uint16_t z)
    {
        value_ = z;
    }

    uint16_t GetValue()
    {
        return value_;
    }
};

class BatchId {
public:
    BatchId():value_(0)
    {
    }
    uint16_t value_;
    void SetValue(uint16_t id)
    {
        value_ = id;
    }

    uint16_t GetValue()
    {
        return value_;
    }
};

static uint32_t PackSortId(uint16_t z, uint16_t b)
{
    uint32_t sid = uint32_t(int32_t(z) + (0xFFFF >> 1));
    sid = (sid << 16) + uint32_t(b);
    return sid;
}

static std::tuple<uint16_t, uint16_t> UnpackSortId(uint32_t sort_id)
{
    auto b = uint16_t(sort_id & 0xFFFF);
    auto z = uint16_t(int32_t(sort_id >> 16) - (0xFFFF >> 1));
    return std::make_tuple(b, z);
}

}
