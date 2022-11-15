#pragma once

#include "stb_image.h"
#include "stb_image_resize.h"
#include <memory>
#include <math/common_func.h>

namespace ant2d {
class ImageData {
    using StbPixelsPtr = std::unique_ptr<stbi_uc, decltype(&stbi_image_free)>;

public:
    ImageData(const uint8_t* data, size_t size);
    ImageData(ImageData&& other);
    ImageData(int width, int height, size_t size, StbPixelsPtr pixels, int num_channels);
    ImageData(const std::string& filename);
    int width_;
    int height_;
    size_t size_;
    StbPixelsPtr pixels_;
    int num_channels_;

    void SetColor(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        uint8_t* old_data = (uint8_t*)pixels_.get();
        uint8_t* src_index = old_data + num_channels_ * (y * width_ + x);
        src_index[0] = r;
        src_index[1] = g;
        src_index[2] = b;
        src_index[3] = a;
    }

    static ImageData Pow2Image(const ImageData& img)
    {
        int old_width = img.width_;
        int old_height = img.height_;

        int new_width = math::Pow2(old_width);
        int new_height = math::Pow2(old_height);

        uint8_t* dst_data = (uint8_t*)malloc(new_width * new_height);
        uint8_t* old_data = (uint8_t*)img.pixels_.get();
        for (int y = 0; y < old_height; y++) {
            for (int x = 0; x < old_width; x++) {
                int offset = img.num_channels_ * (y * old_width + x);
                uint8_t* src_index = old_data + offset;
                uint8_t* dst_index = dst_data + offset;
                std::copy(src_index, src_index + img.num_channels_, dst_index);
            }
        }

        return ImageData { new_width, new_height, new_width * new_height * img.num_channels_,
            StbPixelsPtr { dst_data, stbi_image_free }, img.num_channels_ };
    }

    static ImageData Scale(const ImageData& img, int scale)
    {
        int new_width = math::Pow2(img.width_) * scale;
        int new_height = math::Pow2(img.height_) * scale;
        uint8_t* dst_data = (uint8_t*)malloc(new_width * new_height);
        uint8_t* old_data = (uint8_t*)img.pixels_.get();

        int ret = stbir_resize_uint8(old_data, img.width_, img.height_, 0,
            dst_data, new_width, new_height, 0,
            4);
        assert(ret);
        return ImageData { new_width, new_height, new_width * new_height * 4,
            StbPixelsPtr { dst_data, stbi_image_free }, 4 };
    }

private:
    StbPixelsPtr CreatePixels(const uint8_t* data, size_t size);
};
}
