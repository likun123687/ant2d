#include <effect/chan.h>
#include <math/common_func.h>

namespace ant2d {
class VisualController {
public:
    VisualController()
        : position_ { nullptr, 0 }
        , color_ { nullptr, 0 }
        , particle_size_ { nullptr, 0 }
        , rotation_ { nullptr, 0 }
    {
    }
    Channel_v2 position_;
    Channel_v4 color_;
    Channel_f32 particle_size_;
    Channel_f32 rotation_;

public:
    // Visualize write the Live particles to vertex-buffer.
    void Visualize(std::vector<PosTexColorVertex>& buf, ITexture2D* tex, int live, bool additive);
};

} // namespace ant2d
