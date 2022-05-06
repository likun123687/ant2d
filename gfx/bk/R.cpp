#include <gfx/bk/R.h>
#include <utils/Debug.h>

using namespace bk;

ResManager::ResManager()
{
    ibIndex_ = 1;
    vbIndex_ = 1;
    ttIndex_ = 1;
    umIndex_ = 1;
    shIndex_ = 1;
}

std::tuple<uint16_t, IndexBuffer*> ResManager::AllocIndexBuffer(const void *ptr, size_t size)
{
    uint16_t id = 0;
    IndexBuffer *ib = nullptr;

    auto index = ibFrees_.Pop();
    if (index) {
        id = index;
        ib = &indexBuffers_[index];
    } else {
        id = ibIndex_;
        ib = &indexBuffers_[ibIndex_];
        ibIndex_++;
    }

    id = id | (IdTypeIndex << IdTypeShift);
    ib->Create(ptr, size, 0);

    return std::make_tuple(id, ib);
}

std::tuple<uint16_t, VertexBuffer*> ResManager::AllocVertexBuffer(const void *ptr, size_t size, uint16_t stride)
{
    uint16_t id = 0;
    VertexBuffer *vb = nullptr;

    auto index = vbFrees_.Pop();
    if (index) {
        id = index;
        vb = &vertexBuffers_[index];
    } else {
        id = vbIndex_;
        vb = &vertexBuffers_[vbIndex_];
        vbIndex_++;
    }

    id = id | (IdTypeVertex << IdTypeShift);
    vb->Create(ptr, size, stride, 0);

    return std::make_tuple(id, vb);
}

std::tuple<uint16_t, Uniformblock*> ResManager::AllocUniformblock(uint16_t shId, sg_shader_stage stage, const std::string& name)
{
    uint16_t id = 0;
    Uniformblock *um = nullptr;

    auto index = umFrees_.Pop();
    if (index) {
        id = index;
        um = &uniformsblocks_[index];
    } else {
        id = umIndex_;
        um = &uniformsblocks_[umIndex_];
        umIndex_++;
    }
    id = id | (IdTypeUniformblock << IdTypeShift);

    auto sh = GetShader(shId);
    if (!sh) {
        Error("not found shader id {}", shId&IdMask);
        //return;
    }
    um->Create(sh->GetType(), stage, name);
    return std::make_tuple(id, um);
}

std::tuple<uint16_t, Texture2D*> ResManager::AllocTexture(const ImageData& data)
{
    uint16_t id = 0;
    Texture2D *tex = nullptr;

    auto index = ttFrees_.Pop();
    if (index) {
        id = index;
        tex = &textures_[index];
    } else {
        id = ttIndex_;
        tex = &textures_[ttIndex_];
        ttIndex_++;
    }
    id = id | (IdTypeTexture << IdTypeShift);
    tex->Create(data);
    return std::make_tuple(id, tex);
}

void ResManager::Free(uint16_t id)
{
    auto t = (id >> IdTypeShift) & 0x000F;
    auto v = id & IdMask;

    switch (t) {
        case IdTypeIndex:
            indexBuffers_[v].Destroy();
            ibFrees_.Push(v);
            break;
        case IdTypeVertex:
            vertexBuffers_[v].Destroy();
            vbFrees_.Push(v);
            break;
        case IdTypeTexture:
            textures_[v].Destroy();
            ttFrees_.Push(v);
            break;
        case IdTypeUniformblock:
            umFrees_.Push(v);
            break;
        case IdTypeShader:
            shaders_[v].Destroy();
            shFrees_.Push(v);
            break;
    }
}

IndexBuffer *ResManager::GetIndexBuffer(uint16_t id)
{
    auto t = id>>IdTypeShift;
    auto v = id&IdMask;
    if (t != IdTypeIndex || v >= MaxIndex) {
        return nullptr;
    }
    return &indexBuffers_[v];
}

VertexBuffer *ResManager::GetVertexBuffer(uint16_t id)
{
    auto t = id>>IdTypeShift;
    auto v = id&IdMask;
    if (t != IdTypeVertex || v >= MaxVertex) {
        return nullptr;
    }
    return &vertexBuffers_[v];
}

Texture2D *ResManager::GetTexture(uint16_t id)
{
    auto t = id>>IdTypeShift;
    auto v = id&IdMask;
    if (t != IdTypeTexture || v >= MaxTexture) {
        return nullptr;
    }
    return &textures_[v];
}

Uniformblock *ResManager::GetUniformblock(uint16_t id)
{
    auto t = id>>IdTypeShift;
    auto v = id&IdMask;
    if (t != IdTypeUniformblock || v >= MaxUniformblock) {
        return nullptr;
    }
    return &uniformsblocks_[v];
}

Shader *ResManager::GetShader(uint16_t id)
{
    auto t = id>>IdTypeShift;
    auto v = id&IdMask;
    if (t != IdTypeShader || v >= MaxShader) {
        return nullptr;
    }
    return &shaders_[v];
}
