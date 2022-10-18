#pragma once
#include <gfx/bk/buffer.h>
#include <gfx/bk/pipeline.h>
#include <gfx/bk/shader.h>
#include <gfx/bk/texture.h>
#include <gfx/bk/uniformblock.h>
#include <tuple>
#include <utils/singleton.h>
#include <vector>

namespace ant2d {
constexpr uint16_t kInvalidId = 0x0000;
constexpr uint16_t kUInt16Max = 0xFFFF;

constexpr uint16_t kIdMask = 0x0FFF;
constexpr uint16_t kIdTypeShift = 12;

enum IdType {
    kIdTypeIndex = 0,
    kIdTypeVertex,
    kIdTypeTexture,
    kIdTypeLayout,
    kIdTypeUniformblock,
    kIdTypeShader,
    kIdTypePipeline,
};

constexpr uint16_t kMaxIndex = 2 << 10;
constexpr uint16_t kMaxVertex = 2 << 10;
constexpr uint16_t kMaxTexture = 1 << 10;
constexpr uint16_t kMaxUniformblock = 32 * 8;
constexpr uint16_t kMaxShader = 32;
constexpr uint16_t kMaxPipeline = 32;

class FreeList {
public:
    FreeList()
        : slots_()
    {
    }

    uint16_t Pop()
    {
        uint16_t slot = 0;
        if (!slots_.empty()) {
            slot = slots_.back();
            slots_.pop_back();
        }
        return slot;
    }

    void Push(uint16_t slot)
    {
        slots_.push_back(slot);
    }

private:
    std::vector<uint16_t> slots_;
};

class ResManager {
private:
    std::array<IndexBuffer, kMaxIndex> index_buffers_;
    std::array<VertexBuffer, kMaxVertex> vertex_buffers_;
    std::array<Texture2D, kMaxTexture> textures_;

    std::array<Uniformblock, kMaxUniformblock> uniformsblocks_;
    std::array<Shader, kMaxShader> shaders_;
    std::array<Pipeline, kMaxPipeline> pipelines_;

    uint16_t ib_index_;
    uint16_t vb_index_;
    uint16_t tt_index_;
    uint16_t um_index_;
    uint16_t sh_index_;
    uint16_t pipeline_index_;

    // free list
    FreeList ib_frees_;
    FreeList vb_frees_;
    FreeList tt_frees_;
    FreeList um_frees_;
    FreeList sh_frees_;
    FreeList pipeline_frees_;

public:
    ResManager();
    std::tuple<uint16_t, IndexBuffer*> AllocIndexBuffer(const uint8_t* ptr, size_t size);
    std::tuple<uint16_t, VertexBuffer*> AllocVertexBuffer(const uint8_t* ptr, size_t size, uint16_t stride);
    std::tuple<uint16_t, Uniformblock*> AllocUniformblock(uint16_t shId, sg_shader_stage stage, const std::string& name);
    std::tuple<uint16_t, Texture2D*> AllocTexture(const ImageData& data);
    std::tuple<uint16_t, Shader*> AllocShader(ShaderType type);
    std::tuple<uint16_t, Pipeline*> AllocPipeline(const sg_pipeline_desc* desc);
    void Free(uint16_t id);
    IndexBuffer* GetIndexBuffer(uint16_t id, bool had_trip_type = false);
    VertexBuffer* GetVertexBuffer(uint16_t id, bool had_trip_type = false);
    Texture2D* GetTexture(uint16_t id, bool had_trip_type = false);
    Uniformblock* GetUniformblock(uint16_t id, bool had_trip_type = false);
    Shader* GetShader(uint16_t id, bool had_trip_type = false);
    Pipeline* GetPipeline(uint16_t id, bool had_trip_type = false);
    static uint16_t TripType(uint16_t id)
    {
        return id & kIdMask;
    }
};
}

#define SharedResManager \
    ::ant2d::Singleton<::ant2d::ResManager>()
