#include <gui/ui_render_feature.h>

namespace ant2d {

void UIRenderFeature::AllocBuffer(int isz, int vsz)
{
    if (isz > buffer_.isz) {
        if (buffer_.iid != kInvalidId) {
            SharedResManager.Free(buffer_.iid);
        }

        {
            isz--;
            isz |= isz >> 1;
            isz |= isz >> 2;
            isz |= isz >> 3;
            isz |= isz >> 8;
            isz |= isz >> 16;
            isz++;
        }
        buffer_.isz = isz;

        std::tie(buffer_.iid, buffer_.index) = SharedResManager.AllocIndexBuffer(nullptr, isz * 2);
    }

    std::tie(buffer_.vid, std::ignore, buffer_.vertex) = SharedContext.TempVertexBuffer(vsz, sizeof(PosTexColorVertex));
}

void UIRenderFeature::SetDrawList(DrawList* dl)
{
    draw_list_ = dl;
}

void UIRenderFeature::Register(RenderSystem* rs)
{
    camera_ = rs->GetMainCamera();
    for (auto& r : rs->GetRenderList()) {
        if (r->GetType() == ShaderType::kMeshShader) {
            mesh_render_ = reinterpret_cast<MeshRender*>(r.get());
            break;
        }
    }

    // add new feature, use the index as id
    id_ = rs->Accept(this);
    draw_list_ = &gui::DefaultContext().GetDrawList();
    buffer_.vid = kInvalidId;
    buffer_.iid = kInvalidId;
}

void UIRenderFeature::Extract(View* v)
{
    if (!draw_list_->Empty()) {
        uint32_t fi = uint32_t(id_) << 16;
        DrawCmd* cmd = draw_list_->GetCommands();

        for (int i = 0; i < draw_list_->cmd_index_ - 1; i++) {
            auto z_order = cmd[i].z_order;
            auto sid = PackSortId(z_order, 0);
            auto value = fi + uint32_t(i);
            v->render_nodes.push_back(SortObject { sid, value });
        }
    }
}

void UIRenderFeature::Draw(const std::vector<SortObject>& nodes)
{
    float sw = 0.0f;
    std::tie(sw, std::ignore) = camera_->Screen();
    auto [x, y, w, h] = camera_->GetView();
    auto [sx, sy] = camera_->Scale();

    auto dx = x - w * sx / 2;
    auto dy = y - h * sy / 2;
    auto sk = w * sx / sw;

    // setup buffer
    auto [isz, vsz] = draw_list_->Size();
    if (buffer_.first_draw) {
        buffer_.first_draw = false;
        AllocBuffer(isz, vsz);
        buffer_.vertex->Update((uint8_t*)(draw_list_->vtx_buffer_.data()), vsz * sizeof(PosTexColorVertex), 0, false);
        buffer_.index->Update((uint8_t*)(draw_list_->idx_buffer_.data()), isz * 2, 0, false);
    }

    Mesh mesh {};
    mesh.SetIndexId(buffer_.iid);
    mesh.SetVertexId(buffer_.vid);

    auto mat4 = math::Mat4 { sk, 0, 0, 0, 0, sk, 0, 0, dx, dy, 1, 0, 0, 0, 0, 1 };
    DrawCmd* commands = draw_list_->GetCommands();
    for (auto& b : nodes) {
        auto index = b.value & 0xFFFF; // 在mesh_table索引
        auto& cmd = commands[index];

        mesh.SetFirstVertex(0);
        mesh.SetNumVertex(vsz);
        mesh.SetFirstIndex(cmd.first_index);
        mesh.SetNumIndex(cmd.elem_count);
        mesh.SetTexture(cmd.texture_id);
        mesh_render_->Draw(&mesh, &mat4, cmd.z_order); 
    }
}
void UIRenderFeature::Flush()
{
    auto [isz, vsz] = draw_list_->Size();
    SharedDebug.Hud("gui drawlist:" + std::to_string(isz) + " " + std::to_string(vsz));
    draw_list_->Clear();
    buffer_.first_draw = true;
}
}
