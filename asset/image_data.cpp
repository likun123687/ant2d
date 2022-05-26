#include <asset/image_data.h>
#include <utils/Debug.h>

using namespace ant2d;

ImageData::StbPixelsPtr ImageData::CreatePixels(const uint8_t* data, size_t size)
{
    const int desired_channels = 4;
    auto mem_pixels = stbi_load_from_memory(
            data,
            size,
            &width_, &height_,
            &num_channels_, desired_channels);
    assert(mem_pixels);
    return StbPixelsPtr(mem_pixels, stbi_image_free);
}

ImageData::ImageData(const uint8_t* data, size_t size):pixels_(CreatePixels(data, size))
{
    size_ = size;
}

//ImageData::ImageData(ImageData&& other) :width(other.width), height(other.height),
//                                        size(other.size), pixels(std::move(other.pixels))
//                                        num_channels(other.num_channels)
//}

