#include <gfx/bk/texture.h>

using namespace bk;

void Texture2D::Create(const ImageData& img)
{
    width_ = static_cast<float>(img.width);
    height_ = static_cast<float>(img.height);
    id_ = newTexture_(img);
}

void Texture2D::Update(const ImageData& img)
{
    sg_image_data image_data = {};
    image_data.subimage[0][0].ptr = img.pixels.get();
    image_data.subimage[0][0].size = img.width * img.height * 4;
    return sg_update_image(id_, &image_data);
}

void  Texture2D::Destroy()
{
    return sg_destroy_image(id_);
}

sg_image Texture2D::newTexture_(const ImageData& img)
{
    sg_image image_id = sg_alloc_image();
    sg_image_desc desc = {};
    desc.width = img.width;
    desc.height = img.height;
    desc.pixel_format = SG_PIXELFORMAT_RGBA8;
    desc.wrap_u = SG_WRAP_REPEAT;
    desc.wrap_v = SG_WRAP_REPEAT;
    desc.data.subimage[0][0].ptr = img.pixels.get();
    desc.data.subimage[0][0].size = img.width * img.height * 4;
    sg_init_image(image_id, &desc);
    return image_id;
}

