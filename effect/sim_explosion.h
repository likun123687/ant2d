#pragma once
#include <effect/pool.h>
#include <effect/rate_controller.h>
#include <effect/life_controller.h>
#include <effect/visual_controller.h>

namespace ant2d {
class ExplosionSimulator : public ISimulator {
private:
    Pool pool_;
    std::unique_ptr<RateController> rate_controller_ { new RateController() };
    std::unique_ptr<LifeController> life_controller_ { new LifeController() };
    std::unique_ptr<VisualController> visual_controller_ { new VisualController() };
    Channel_v2 velocity_;
    Channel_v4 delta_color_;
    // Configuration.
    struct Config {
        float duration;
        float rate;
        Var life;
        Var size;
        TwoColor color;
        std::array<Var, 2> position;
        Var angle;
        Var speed;
        bool additive;
    };

    Config config_;

    void NewParticle(int new_particle);

public:
    ExplosionSimulator(int cap);

    void Initialize();

    void Simulate(float dt);

    std::tuple<int, int> Size();
    void Visualize(PosTexColorVertex* buf, ITexture2D* tex);
    IController* GetController()
    {
        return rate_controller_.get();
    }
};

} // namespace ant2d
