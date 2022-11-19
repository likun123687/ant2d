#pragma once

#include "stb_image.h"
#include "stb_image_resize.h"
#include <memory>
#include <math/common_func.h>

namespace ant2d {
class ImageData {
public:
    using StbPixelsPtr = std::unique_ptr<stbi_uc, decltype(&stbi_image_free)>;

    ImageData(const uint8_t* data, size_t size);
    ImageData(ImageData&& other);
    ImageData(int width, int height, size_t size, StbPixelsPtr pixels, int num_channels);
    ImageData(const std::string& filename);
    ImageData(int width, int height, int num_channels);
    int width_;
    int height_;
    size_t size_;
    StbPixelsPtr pixels_;
    int num_channels_;

    void SetColor(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    static ImageData Pow2Image(const ImageData& img);
    static ImageData Scale(const ImageData& img, int scale);

private:
    StbPixelsPtr CreatePixels(const uint8_t* data, size_t size);
};
}
