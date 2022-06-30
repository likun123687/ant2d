#include <gfx/bk/res_manager.h>
#include <utils/debug.h>

using namespace ant2d;

ResManager::ResManager()
{
    ib_index_ = 1;
    vb_index_ = 1;
    tt_index_ = 1;
    um_index_ = 1;
    sh_index_ = 1;
    pipeline_index_ = 1;
}

std::tuple<uint16_t, IndexBuffer*> ResManager::AllocIndexBuffer(const uint8_t* ptr, size_t size)
{
    uint16_t id = 0;
    IndexBuffer* ib = nullptr;

    auto index = ib_frees_.Pop();
    if (index) {
        id = index;
        ib = &index_buffers_[index];
    } else {
        id = ib_index_;
        ib = &index_buffers_[ib_index_];
        ib_index_++;
    }

    id = id | (kIdTypeIndex << kIdTypeShift);
    Info("alloc index id {}", id);
    ib->Create(ptr, size, 0);

    return std::make_tuple(id, ib);
}

std::tuple<uint16_t, VertexBuffer*> ResManager::AllocVertexBuffer(const uint8_t* ptr, size_t size, uint16_t stride)
{
    uint16_t id = 0;
    VertexBuffer* vb = nullptr;

    auto index = vb_frees_.Pop();
    if (index) {
        id = index;
        vb = &vertex_buffers_[index];
    } else {
        id = vb_index_;
        vb = &vertex_buffers_[vb_index_];
        vb_index_++;
    }

    id = id | (kIdTypeVertex << kIdTypeShift);
    vb->Create(ptr, size, stride, 0);

    return std::make_tuple(id, vb);
}

std::tuple<uint16_t, Uniformblock*> ResManager::AllocUniformblock(uint16_t shId, sg_shader_stage stage, const std::string& name)
{
    uint16_t id = 0;
    Uniformblock* um = nullptr;

    auto index = um_frees_.Pop();
    if (index) {
        id = index;
        um = &uniformsblocks_[index];
    } else {
        id = um_index_;
        um = &uniformsblocks_[um_index_];
        um_index_++;
    }
    id = id | (kIdTypeUniformblock << kIdTypeShift);

    auto sh = GetShader(shId);
    if (!sh) {
        Error("not found shader id {}", shId & kIdMask);
        // return;
    }
    um->Create(sh->GetType(), stage, name);
    return std::make_tuple(id, um);
}

std::tuple<uint16_t, Texture2D*> ResManager::AllocTexture(const ImageData& data)
{
    uint16_t id = 0;
    Texture2D* tex = nullptr;

    auto index = tt_frees_.Pop();
    if (index) {
        id = index;
        tex = &textures_[index];
    } else {
        id = tt_index_;
        tex = &textures_[tt_index_];
        tt_index_++;
    }
    id = id | (kIdTypeTexture << kIdTypeShift);
    tex->Create(data);
    return std::make_tuple(id, tex);
}

std::tuple<uint16_t, Shader*> ResManager::AllocShader(ShaderType type)
{
    uint16_t id = 0;
    Shader* shader = nullptr;

    auto index = sh_frees_.Pop();
    if (index) {
        id = index;
        shader = &shaders_[index];
    } else {
        id = sh_index_;
        shader = &shaders_[sh_index_];
        sh_index_++;
    }
    id = id | (kIdTypeShader << kIdTypeShift);
    shader->Create(type);

    return std::make_tuple(id, shader);
}

std::tuple<uint16_t, Pipeline*> ResManager::AllocPipeline(const sg_pipeline_desc *desc)
{
    uint16_t id = 0;
    Pipeline* pipeline = nullptr;

    auto index = pipeline_frees_.Pop();
    if (index) {
        id = index;
        pipeline = &pipelines_[index];
    } else {
        id = pipeline_index_;
        pipeline = &pipelines_[pipeline_index_];
        pipeline_index_++;
    }
    id = id | (kIdTypePipeline << kIdTypeShift);
    pipeline->Create(desc);
    return std::make_tuple(id, pipeline);
}

void ResManager::Free(uint16_t id)
{
    auto t = (id >> kIdTypeShift) & 0x000F;
    auto v = id & kIdMask;

    switch (t) {
    case kIdTypeIndex:
        index_buffers_[v].Destroy();
        ib_frees_.Push(v);
        break;
    case kIdTypeVertex:
        vertex_buffers_[v].Destroy();
        vb_frees_.Push(v);
        break;
    case kIdTypeTexture:
        textures_[v].Destroy();
        tt_frees_.Push(v);
        break;
    case kIdTypeUniformblock:
        um_frees_.Push(v);
        break;
    case kIdTypeShader:
        shaders_[v].Destroy();
        sh_frees_.Push(v);
        break;
    case kIdTypePipeline:
        pipelines_[v].Destroy();
        pipeline_frees_.Push(v);
        break;
    }
}

IndexBuffer* ResManager::GetIndexBuffer(uint16_t id, bool had_trip_type)
{
    if (had_trip_type) {
        return &index_buffers_[id];
    }

    auto t = id >> kIdTypeShift;
    auto v = id & kIdMask;
    if (t != kIdTypeIndex || v >= kMaxIndex) {
        return nullptr;
    }
    return &index_buffers_[v];
}

VertexBuffer* ResManager::GetVertexBuffer(uint16_t id, bool had_trip_type)
{
    if (had_trip_type) {
        return &vertex_buffers_[id];
    }
    auto t = id >> kIdTypeShift;
    auto v = id & kIdMask;
    if (t != kIdTypeVertex || v >= kMaxVertex) {
        return nullptr;
    }
    return &vertex_buffers_[v];
}

Texture2D* ResManager::GetTexture(uint16_t id, bool had_trip_type)
{
    if (had_trip_type) {
        return &textures_[id];
    }
    auto t = id >> kIdTypeShift;
    auto v = id & kIdMask;
    if (t != kIdTypeTexture || v >= kMaxTexture) {
        return nullptr;
    }
    return &textures_[v];
}

Uniformblock* ResManager::GetUniformblock(uint16_t id, bool had_trip_type)
{
    if (had_trip_type) {
        return &uniformsblocks_[id];
    }
    auto t = id >> kIdTypeShift;
    auto v = id & kIdMask;
    if (t != kIdTypeUniformblock || v >= kMaxUniformblock) {
        return nullptr;
    }
    return &uniformsblocks_[v];
}

Shader* ResManager::GetShader(uint16_t id, bool had_trip_type)
{
    if (had_trip_type) {
        return &shaders_[id];
    }
    auto t = id >> kIdTypeShift;
    auto v = id & kIdMask;
    if (t != kIdTypeShader || v >= kMaxShader) {
        return nullptr;
    }
    return &shaders_[v];
}

Pipeline* ResManager::GetPipeline(uint16_t id, bool had_trip_type)
{
    if (had_trip_type) {
        return &pipelines_[id];
    }
    auto t = id >> kIdTypeShift;
    auto v = id & kIdMask;
    if (t != kIdTypePipeline || v >= kMaxPipeline) {
        return nullptr;
    }
    return &pipelines_[v];
}
