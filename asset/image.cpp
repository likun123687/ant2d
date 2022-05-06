#include <asset/image.h>
#include <utils/Debug.h>

using namespace ant2d;

ImageData::stb_pixels_p ImageData::createPixels_(const uint8_t* data, size_t size)
{
    const int desired_channels = 4;
    auto mem_pixels = stbi_load_from_memory(
            data,
            size,
            &(this->width), &(this->height),
            &(this->num_channels), desired_channels);
    assert(mem_pixels);
    return stb_pixels_p(mem_pixels, stbi_image_free);
}

ImageData::ImageData(const uint8_t* data, size_t size):pixels(createPixels_(data, size))
{
    this->size = size;
}

//ImageData::ImageData(ImageData&& other) :width(other.width), height(other.height),
//                                        size(other.size), pixels(std::move(other.pixels))
//                                        num_channels(other.num_channels)
//}

