#include <asset/image_data.h>
#include <utils/debug.h>
#include <utils/content.h>

using namespace ant2d;

ImageData::StbPixelsPtr ImageData::CreatePixels(const uint8_t* data, size_t size)
{
    constexpr int desired_channels = 4;
    auto mem_pixels = stbi_load_from_memory(
        data,
        size,
        &width_, &height_,
        &num_channels_, desired_channels);
    assert(mem_pixels);
    return StbPixelsPtr(mem_pixels, stbi_image_free);
}

ImageData::ImageData(const uint8_t* data, size_t size)
    : pixels_(CreatePixels(data, size))
{
    size_ = size;
}

ImageData::ImageData(ImageData&& other)
    : width_(other.width_)
    , height_(other.height_)
    , size_(other.size_)
    , pixels_(std::move(other.pixels_))
    , num_channels_(other.num_channels_)
{
}

ImageData::ImageData(int width, int height, size_t size, StbPixelsPtr pixels, int num_channels)
    : width_(width)
    , height_(height)
    , size_(size)
    , pixels_(std::move(pixels))
    , num_channels_(num_channels)
{
}

ImageData::ImageData(const std::string& filename)
    : pixels_ { nullptr, stbi_image_free }
{
    auto full_path = SharedContent.GetFullPath(filename);
    uint8_t* data = stbi_load(full_path.c_str(), &width_, &height_, &num_channels_, 4);
    assert(data);
    pixels_.reset(data);
}
