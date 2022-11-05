#include <effect/pool.h>
#include <effect/rate_controller.h>
#include <effect/life_controller.h>
#include <effect/visual_controller.h>

namespace ant2d {

// SnowSimulator can simulate snow effect.
class SnowSimulator : public Simulator {
    struct Config {
        float duration;
        float rate;
        Var life;
        Var size;
        math::Vec4 color;
        std::array<Var, 2> position;
        std::array<Var, 2> velocity;
        Var rotation;
    };

private:
    Pool pool_;
    RateController rate_controller_;
    LifeController life_controller_;
    VisualController visual_controller_;

    Channel_v2 velocity_;
    Channel_f32 delta_rot_;
    Config config_;

    void NewParticle(int new_particle);

public:
    SnowSimulator(int cap, float w, float h);

    void Initialize();

    void Simulate(float dt);

    std::tuple<int, int> Size();
    void Visualize(std::vector<PosTexColorVertex>& buf, ITexture2D* tex);
};

} // namespace ant2d
