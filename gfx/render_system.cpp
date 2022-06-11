#include <algorithm>
#include <gfx/render_system.h>
#include <gfx/transform/transform_table.h>

namespace ant2d {

RenderSystem::RenderSystem()
    : main_camera_(new Camera())
{
    main_camera_->SetFollow(Ghost);
    main_camera_->Init();
    view_.camera = main_camera_.get();
    // view_.RenderNodes = std::vector<BaseTable>();
}

void RenderSystem::RequireTable(TableList& tables)
{
    table_list_ = &tables;
    for (auto& table : tables) {
        if (table->GetTableType() == TableType::kTransform) {
            transform_table_ = reinterpret_cast<TransformTable*>(table.get());
            break;
        }
    }
}

int RenderSystem::Accept(IRenderFeature* rf)
{
    int index = (int)(feature_list_.size());
    feature_list_.emplace_back(rf);
    return index;
}

void RenderSystem::RegisterRender(IRender* render)
{
    render_list_.emplace_back(render);
}

void RenderSystem::Update(float dt)
{
    // update camera
    auto follow = main_camera_->GetFollow();
    if (!follow.IsGhost()) {
        auto xf = transform_table_->GetComp(follow);
        auto p = xf->GetPosition();
        auto mat = main_camera_->GetMat();
        auto dx = (p[0] - mat.x) * 1;
        auto dy = (p[1] - mat.y) * 1;
        main_camera_->MoveBy(dx, dy);
    }

    // main camera
    for (auto& r : render_list_) {
        r->SetCamera(main_camera_.get());
    }

    // extract
    for (auto& f : feature_list_) {
        f->Extract(&view_);
    }

    // sort
    auto nodes = view_.render_nodes;
    auto n = nodes.size();
    std::sort(nodes.begin(), nodes.end(),
        [](const SortObject& a, const SortObject& b) -> bool {
            return a.sort_id < b.sort_id;
        });

    // draw
    int i = 0;
    int j = 0;
    for (; i < n; i = j) {
        auto fi = (nodes[i].value >> 16);
        j = i + 1;
        while (j < n && (nodes[j].value >> 16 == fi)) {
            j++;
        }
        auto& f = feature_list_[fi];
        // f.Draw(v.RenderNodes [i:j]);
        f->Draw({ view_.render_nodes.begin() + i, view_.render_nodes.begin() + j });
    }

    // flush, release any resource
    for (auto& f : feature_list_) {
        f->Flush();
    }

    // view reset
    view_.render_nodes.clear();
}

void RenderSystem::Destroy()
{
}

Camera* RenderSystem::GetMainCamera()
{
    return main_camera_.get();
}

} // namespace ant2d
