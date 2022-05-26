#pragma once
#include <asset/image_data.h>
#include <gfx/bk/sokol_gfx_bk.h>

namespace ant2d {
class Texture2D {
public:
    void Create(const ImageData& img);
    void Update(const ImageData& img);
    void Destroy();
    sg_image GetId() const;

private:
    sg_image NewTexture(const ImageData& img);
    float width_;
    float height_;
    sg_image id_;
};
}
