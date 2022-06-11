#pragma once
#include <gfx/camera.h>
#include <gfx/render_data.h>
#include <engi/base_table.h>

#include <memory>

namespace ant2d {
class RenderSystem {
    using RenderList = std::vector<std::unique_ptr<IRender>>;
private:
    std::unique_ptr<Camera> main_camera_;
    View view_;
    TransformTable* transform_table_;
    TableList* table_list_;
    RenderList render_list_;
    std::vector<std::unique_ptr<IRenderFeature>> feature_list_;

public:
    RenderSystem();
    void RequireTable(TableList& tables);
    int Accept(IRenderFeature* rf);
    void RegisterRender(IRender* render);
    void Update(float dt);
    void Destroy();
    Camera *GetMainCamera();
    RenderList& GetRenderList()
    {
        return render_list_;
    }

    TableList* GetTableList()
    {
        return table_list_;
    }
};
} //namespace ant2d
