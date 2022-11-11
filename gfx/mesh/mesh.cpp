#include <gfx/mesh/mesh.h>
#include <utils/debug.h>

namespace ant2d {
void Mesh::Setup()
{
    uint16_t stride = sizeof(PosTexColorVertex);
    std::tie(vertex_id_, std::ignore) = SharedResManager.AllocVertexBuffer((uint8_t*)(vertex_.data()), vertex_.size() * stride, stride);
    if (vertex_id_ == kInvalidId) {
        Error("alloc vertex buffer error");
    }

    std::tie(index_id_, std::ignore) = SharedResManager.AllocIndexBuffer((uint8_t*)(index_.data()), index_.size() * 2);
    if (index_id_ == kInvalidId) {
        Error("alloc vertex buffer error");
    }

    first_vertex_ = 0;
    num_vertex_ = vertex_.size();
    first_index_ = 0;
    num_index_ = index_.size();
}

void Mesh::SetTexture(uint16_t id)
{
    texture_id_ = id;
}

uint16_t Mesh::GetTexture()
{
    return texture_id_;
}

void Mesh::SetVertex(std::vector<PosTexColorVertex> v)
{
    vertex_ = v;
}

void Mesh::SetIndex(std::vector<uint16_t> index)
{
    index_ = index;
}

uint16_t Mesh::GetVertexId()
{
    return vertex_id_;
}

uint16_t Mesh::GetIndexId()
{
    return index_id_;
}

uint16_t Mesh::GetFirstIndex()
{
    return first_index_;
}

uint16_t Mesh::GetNumIndex()
{
    return num_index_;
}

void Mesh::SetVertexId(uint16_t vertex_id)
{
    vertex_id_ = vertex_id;
}

void Mesh::SetIndexId(uint16_t index_id)
{
    index_id_ = index_id;
}

void Mesh::SetFirstVertex(uint16_t first_vertex)
{
    first_vertex_ = first_vertex;
}
void Mesh::SetNumVertex(uint16_t num_vertex)
{
    num_vertex_ = num_vertex;
}

void Mesh::SetFirstIndex(uint16_t first_index)
{
    first_index_ = first_index;
}
void Mesh::SetNumIndex(uint16_t num_index)
{
    num_index_ = num_index;
}

void Mesh::SetVertexOffset(uint32_t offset)
{
    vertex_offset_ = offset;
}

uint32_t Mesh::GetVertexOffset()
{
    return vertex_offset_;
}

void Mesh::Update()
{
    IndexBuffer* index_buffer = SharedResManager.GetIndexBuffer(index_id_);
    if (index_buffer) {
        index_buffer->Update((uint8_t*)index_.data(), index_.size() * sizeof(uint16_t), 0, false);
    }

    auto vertex_buffer = SharedResManager.GetVertexBuffer(vertex_id_);
    if (vertex_buffer) {
        vertex_buffer->Update((uint8_t*)vertex_.data(), vertex_.size() * sizeof(PosTexColorVertex), 0, false);
    }
}

void Mesh::Delete()
{
    auto ib = SharedResManager.GetIndexBuffer(index_id_);
    if (ib) {
        ib->Destroy();
    }

    auto vb = SharedResManager.GetVertexBuffer(vertex_id_);
    if (vb) {
        vb->Destroy();
    }

    auto tex = SharedResManager.GetTexture(texture_id_);
    if (tex) {
        tex->Destroy();
    }
}

}