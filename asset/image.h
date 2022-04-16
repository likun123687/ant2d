namespace asset {
    class ImageData {
        public:
            ImageData(const Uint8* data, Sint64 size):
            {
            const int desired_channels = 4;
            pixels_ = stbi_load_from_memory(
                data,
                (int)size,
                &width_, &width_,
                &num_channels, desired_channels);
            }
            ~ImageData()
            {
                if (pixels_) {
                    stbi_image_free(pixels);
                }
            }
            int width_, height_, nrChannels_;
            unsigned char *data_;
            Sint64 size_;
            stbi_uc* pixels_;
            int num_channels_;
    }
}
