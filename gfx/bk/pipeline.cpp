#include <gfx/bk/pipeline.h>
#include <utils/debug.h>

namespace ant2d {
void Pipeline::Create(const sg_pipeline_desc* desc)
{
    Info("pipe desc {}", static_cast<const void*>(desc));
    id_ = sg_make_pipeline(desc);
}

sg_pipeline Pipeline::GetPipeLineId()
{
    return id_;
}

void Pipeline::Destroy()
{
    sg_destroy_pipeline(id_);
}
}
