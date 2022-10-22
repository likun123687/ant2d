#include <vector>
#include <gfx/render_data.h>
#include <gfx/bk/res_manager.h>

namespace ant2d {
class Mesh {
    std::vector<PosTexColorVertex> vertex_;
    std::vector<uint16_t> index_;

    // res handle
    uint16_t texture_id_;
    uint16_t padding_;

    uint16_t index_id_;
    uint16_t vertex_id_;

    uint16_t first_vertex_;
    uint16_t num_vertex_;

    uint16_t first_index_;
    uint16_t num_index_;

    void Setup()
    {
        uint16_t stride = sizeof(PosTexColorVertex);
        std::tie(vertex_id_, std::ignore) = SharedResManager.AllocVertexBuffer(vertex_.data(), vertex_.size() * stride, stride);
        if (vertex_id_ == kInvalidId) {
            Error("alloc vertex buffer error");
        }

        std::tie(index_id_, std::ignore) = SharedResManager.AllocIndexBuffer(index_.data(), index_.size() * 2);
        if (index_id_ == kInvalidId) {
            Error("alloc vertex buffer error");
        }

        first_vertex_ = 0;
        num_vertex_ = vertex_.size();
        first_index_ = 0;
        num_index_ = index_.size();
    }

    void SetTexture(uint16_t id)
    {
        texture_id_ = id;
    }

    uint16_t GetTexture()
    {
        return texture_id_;
    }

    void SetVertex(std::vector<PosTexColorVertex> v)
    {
        vertex_ = v;
    }

    void SetIndex(std::vector<uint16_t> index)
    {
        index_ = index
    }

    uint16_t GetVertexId()
    {
        return vertex_id_;
    }

    uint16_t GetIndexId()
    {
        return index_id_;
    }

    void Update()
    {
        IndexBuffer* index_buffer = SharedResManager.GetIndexBuffer(index_id_);
        if (index_buffer) {
            index_buffer->Update(index_.data(), index_.size() * sizeof(uint16_t), 0, false);
        }

        auto vertex_buffer = SharedResManager.GetVertexBuffer(vertex_id_);
        if (vertex_buffer) {
            vertex_buffer->Update(vertex_.data(), vertex_.size() * sizeof(PosTexColorVertex), 0, false);
        }
    }

    void Delete()
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
};
}