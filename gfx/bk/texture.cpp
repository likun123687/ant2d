#include <gfx/bk/texture.h>

using namespace ant2d;

void Texture2D::Create(const ImageData& img)
{
    width_ = static_cast<float>(img.width_);
    height_ = static_cast<float>(img.height_);
    id_ = NewTexture(img);
}

void Texture2D::Update(const ImageData& img)
{
    sg_image_data image_data = {};
    image_data.subimage[0][0].ptr = img.pixels_.get();
    image_data.subimage[0][0].size = img.width_ * img.height_ * img.num_channels_;
    return sg_update_image(id_, &image_data);
}

void Texture2D::Destroy()
{
    return sg_destroy_image(id_);
}

sg_image Texture2D::GetId() const
{
    return id_;
}

sg_image Texture2D::NewTexture(const ImageData& img)
{
    sg_image image_id = sg_alloc_image();
    sg_image_desc desc = {};
    desc.width = img.width_;
    desc.height = img.height_;
    desc.pixel_format = SG_PIXELFORMAT_RGBA8;
    desc.wrap_u = SG_WRAP_REPEAT;
    desc.wrap_v = SG_WRAP_REPEAT;
    desc.data.subimage[0][0].ptr = img.pixels_.get();
    desc.data.subimage[0][0].size = img.width_ * img.height_ * img.num_channels_;
    sg_init_image(image_id, &desc);
    return image_id;
}

float Texture2D::GetWidth()
{
    return width_;
}

float Texture2D::GetHeight()
{
    return height_;
}
