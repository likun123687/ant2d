#pragma once
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

    void Setup();

    void SetTexture(uint16_t id);

    uint16_t GetTexture();
    void SetVertex(std::vector<PosTexColorVertex> v);
    void SetIndex(std::vector<uint16_t> index);

    uint16_t GetVertexId();

    uint16_t GetIndexId();

    void Update();

    void Delete();
};
}