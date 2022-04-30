#include <asset/image.h>
#include <gfx/bk/sokol_gfx_bk.h>

using namespace ant2d;

namespace bk {
    class Texture2D
    {
        public:
            void Create(const ImageData& img);
            void Update(const ImageData& img);
            void Destroy();

        private:
            sg_image newTexture_(const ImageData& img);
            float width_;
            float height_;
            sg_image id_;
    };
}
