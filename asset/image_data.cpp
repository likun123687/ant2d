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

ImageData::ImageData(int width, int height, int num_channels)
    : width_(width)
    , height_(height)
    , size_(width * height * num_channels)
    , num_channels_(num_channels)
    , pixels_ { nullptr, stbi_image_free }
{
    uint8_t* data = (uint8_t*)calloc(width * height, num_channels);
    assert(data);
    pixels_.reset(data);
}

void ImageData::SetColor(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    uint8_t* old_data = (uint8_t*)pixels_.get();
    uint8_t* src_index = old_data + num_channels_ * (y * width_ + x);
    src_index[0] = r;
    src_index[1] = g;
    src_index[2] = b;
    src_index[3] = a;
}

ImageData ImageData::Pow2Image(const ImageData& img)
{
    int old_width = img.width_;
    int old_height = img.height_;

    int new_width = math::Pow2(old_width);
    int new_height = math::Pow2(old_height);

    uint8_t* dst_data = (uint8_t*)malloc(new_width * new_height * img.num_channels_);
    uint8_t* old_data = (uint8_t*)img.pixels_.get();
    for (int y = 0; y < old_height; y++) {
        for (int x = 0; x < old_width; x++) {
            int offset = img.num_channels_ * (y * old_width + x);
            uint8_t* src_index = old_data + offset;
            uint8_t* dst_index = dst_data + offset;
            std::copy(src_index, src_index + img.num_channels_, dst_index);
        }
    }

    return ImageData { new_width, new_height, static_cast<size_t>(new_width * new_height * img.num_channels_),
        StbPixelsPtr { dst_data, stbi_image_free }, img.num_channels_ };
}

ImageData ImageData::Scale(const ImageData& img, int scale)
{
    int new_width = math::Pow2(img.width_) * scale;
    int new_height = math::Pow2(img.height_) * scale;
    uint8_t* dst_data = (uint8_t*)malloc(new_width * new_height * img.num_channels_);
    uint8_t* old_data = (uint8_t*)img.pixels_.get();

    int ret = stbir_resize_uint8(old_data, img.width_, img.height_, 0,
        dst_data, new_width, new_height, 0,
        4);
    assert(ret);
    return ImageData { new_width, new_height, static_cast<size_t>(new_width * new_height * 4),
        StbPixelsPtr { dst_data, stbi_image_free }, 4 };
}