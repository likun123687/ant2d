#pragma once
#include <effect/pool.h>
#include <effect/rate_controller.h>
#include <effect/life_controller.h>
#include <effect/visual_controller.h>
#include <effect/config.h>

namespace ant2d {

// GravityConfig used to configure the GravitySimulator.
struct GravityConfig : public CommonConfig {
    // gravity
    math::Vec2 gravity;

    // speed and direction
    Var speed;
    Var angel;

    // Radial acceleration
    Var radial_acc;

    // tangent acceleration
    Var tangential_acc;

    bool rotation_is_dir;
};

// GravitySimulator works as the gravity mode of Cocos2D's particle-system.
class GravitySimulator : public ISimulator {

private:
    Pool pool_;
    std::unique_ptr<RateController> rate_controller_ { new RateController() };
    std::unique_ptr<LifeController> life_controller_ { new LifeController() };
    std::unique_ptr<VisualController> visual_controller_ { new VisualController() };

    Channel_v2 pose_start_;
    Channel_v4 color_delta_;
    Channel_f32 size_delta_;
    Channel_f32 rot_;
    Channel_f32 rot_delta_;

    Channel_v2 velocity_;
    Channel_f32 radial_acc_;
    Channel_f32 tangential_acc_;

    math::Vec2 gravity_;

    GravityConfig config_;

    void NewParticle(int new_particle);

public:
    GravitySimulator(GravityConfig cfg);

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
