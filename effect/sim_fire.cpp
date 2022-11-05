#include <effect/sim_fire.h>

namespace ant2d {
void FireSimulator::NewParticle(int new_particle)
{
    if (life_controller_.live_ + new_particle > pool_.GetCap()) {
        return;
    }

    auto start = life_controller_.live_;
    life_controller_.live_ += new_particle;

    for (int i = start; i < life_controller_.live_; i++) {
        life_controller_.life_[i] = config_.life.Random();
        visual_controller_.particle_size_[i] = config_.size.Random();
        visual_controller_.color_[i] = config_.color;
        auto inv_life = 1.0 / life_controller_.life_[i];

        delta_color_[i] = math::Vec4 {
            -config_.color[0] * float(inv_life),
            -config_.color[1] * float(inv_life),
            -config_.color[2] * float(inv_life),
            0.0f,
        };

        auto px = config_.position[0].Random();
        auto py = config_.position[1].Random();

        visual_controller_.position_[i] = math::Vec2 { px, py };

        auto a = config_.angle.Random();
        auto s = config_.speed.Random();
        velocity_[i] = math::Vec2 { math::Cos(a) * s, math::Sin(a) * s };
    }
}

FireSimulator::FireSimulator(int cap)
    : pool_ { cap }
{
    pool_.AddChan({ LIFE_CONST, SIZE_CONST });
    pool_.AddChan({ POSITION_CONST, VELOCITY_CONST });
    pool_.AddChan({ COLOR_CONST });
    pool_.AddChan({ ROTATION_CONST });
    pool_.AddChan({ COLORDELTA_CONST });

    // config
    config_.duration = math::MaxFloat32;
    config_.rate = float(cap) / 3;
    config_.life = Var { 3, 0.25 };
    config_.color = math::Vec4 { .76, .25, .12, 1 };
    config_.size = Var { 34, 10 };
    config_.position[0] = Var { 0, 20 };
    config_.position[1] = Var { 0, 20 };

    config_.angle = Var { 3.14 / 2, 0.314 };
    config_.speed = Var { 60, 20 };
    config_.additive = true;
}

void FireSimulator::Initialize()
{
    pool_.Initialize();
    life_controller_.life_ = std::get<1>(pool_.GetField(LIFE_CONST));

    visual_controller_.particle_size_ = std::get<1>(pool_.GetField(SIZE_CONST));
    visual_controller_.position_ = std::get<2>(pool_.GetField(POSITION_CONST));
    velocity_ = std::get<2>(pool_.GetField(VELOCITY_CONST));

    visual_controller_.color_ = std::get<3>(pool_.GetField(COLOR_CONST));
    delta_color_ = std::get<3>(pool_.GetField(COLORDELTA_CONST));

    visual_controller_.rotation_ = std::get<1>(pool_.GetField(ROTATION_CONST));

    rate_controller_.Initialize(config_.duration, config_.rate);
}

void FireSimulator::Simulate(float dt)
{
    // spawn new particle
    auto new_particle = rate_controller_.Rate(dt);
    if (new_particle > 0) {
        NewParticle(new_particle);
    }

    int32_t n = life_controller_.live_;

    // update old particle
    life_controller_.life_.Sub(n, dt);

    // position integrate: p' = p + v * t
    visual_controller_.position_.Integrate(n, velocity_, dt);

    // color
    visual_controller_.color_.Integrate(n, delta_color_, dt);

    // Gc
    life_controller_.GC(&pool_);
}

std::tuple<int, int> FireSimulator::Size()
{
    return std::make_tuple(int(life_controller_.live_), pool_.GetCap());
}

void FireSimulator::Visualize(std::vector<PosTexColorVertex>& buf, ITexture2D* tex)
{
    visual_controller_.Visualize(buf, tex, life_controller_.live_, config_.additive);
}
} // namespace ant2d
