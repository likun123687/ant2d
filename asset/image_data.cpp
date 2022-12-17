#include <asset/image_data.h>
#include <utils/debug.h>
#include <utils/content.h>
#include <algorithm>

using namespace ant2d;

ImageData::StbPixelsPtr ImageData::CreatePixels(const uint8_t* filedata, size_t size)
{
    constexpr int desired_channels = 4;
    auto mem_pixels = stbi_load_from_memory(
        filedata,
        size,
        &width_, &height_,
        &num_channels_, desired_channels);
    assert(mem_pixels);
    return StbPixelsPtr(mem_pixels, stbi_image_free);
}

ImageData::ImageData(const uint8_t* filedata, size_t size)
    : pixels_(CreatePixels(filedata, size))
{
}

ImageData::ImageData(ImageData&& other)
    : width_(other.width_)
    , height_(other.height_)
    , pixels_(std::move(other.pixels_))
    , num_channels_(other.num_channels_)
{
}

ImageData::ImageData(int width, int height, size_t size, StbPixelsPtr pixels, int num_channels)
    : width_(width)
    , height_(height)
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

    uint8_t* dst_data = (uint8_t*)calloc(new_width * new_height * img.num_channels_, 1);
    uint8_t* old_data = (uint8_t*)img.pixels_.get();

    for (size_t y = 0; y < old_height; y++) {
        for (size_t x = 0; x < old_width; x++) {
            for (size_t c = 0; c < 4; c++) {
                dst_data[4 * new_width * y + 4 * x + c] = old_data[4 * old_width * y + 4 * x + c];
            }
        }
    }

    // stbi_write_png("aaaa.png", old_width, old_height, 4, old_data, 0);
    // stbi_write_png("bbbbb.png", new_width, new_height, 4, dst_data, 0);

    return ImageData { new_width, new_height, static_cast<size_t>(new_width * new_height * img.num_channels_),
        StbPixelsPtr { dst_data, stbi_image_free }, img.num_channels_ };
}

ImageData ImageData::Scale(const ImageData& img, int scale)
{
    int new_width = math::Pow2(img.width_) * scale;
    int new_height = math::Pow2(img.height_) * scale;
    uint8_t* dst_data = (uint8_t*)calloc(new_width * new_height * img.num_channels_, 1);
    uint8_t* old_data = (uint8_t*)img.pixels_.get();

    int ret = stbir_resize_uint8(old_data, img.width_, img.height_, 0,
        dst_data, new_width, new_height, 0,
        4);
    assert(ret);
    return ImageData { new_width, new_height, static_cast<size_t>(new_width * new_height * 4),
        StbPixelsPtr { dst_data, stbi_image_free }, 4 };
}