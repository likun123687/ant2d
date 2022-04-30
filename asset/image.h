#include<memory>
#include "stb_image.h"

namespace ant2d {
    class ImageData
    {
        typedef std::unique_ptr<stbi_uc,  decltype(&stbi_image_free)> stb_pixels_p;

        public:
            ImageData(const uint8_t* data, int64_t size);
            //ImageData(ImageData&& other);
            int width;
            int height;
            int64_t size;
            stb_pixels_p pixels;
            int num_channels;
        private:
            stb_pixels_p createPixels_(const uint8_t* data, int64_t size);
    };
}
