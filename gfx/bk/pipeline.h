#pragma once
#include <gfx/bk/sokol_gfx_bk.h>

//依赖shader_id, attribute
namespace ant2d {
class Pipeline {
private:
    sg_pipeline id_;

public:
    void Create(const sg_pipeline_desc* desc);
    sg_pipeline GetPipeLineId();
    void Destroy();
};
}
