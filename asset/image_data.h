#pragma once

#include "stb_image.h"
#include <memory>

namespace ant2d {
class ImageData {
    using StbPixelsPtr = std::unique_ptr<stbi_uc, decltype(&stbi_image_free)>;

public:
    ImageData(const uint8_t* data, size_t size);
    // ImageData(ImageData&& other);
    int width_;
    int height_;
    size_t size_;
    StbPixelsPtr pixels_;
    int num_channels_;

private:
    StbPixelsPtr CreatePixels(const uint8_t* data, size_t size);
};
}
