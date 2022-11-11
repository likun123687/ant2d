#include <effect/sim_fountain.h>

namespace ant2d {
void FountainSimulator::NewParticle(int new_particle)
{
    if (life_controller_->live_ + new_particle > pool_.GetCap()) {
        return;
    }

    auto start = life_controller_->live_;
    life_controller_->live_ += new_particle;

    for (int i = start; i < life_controller_->live_; i++) {
        life_controller_->life_[i] = config_.life.Random();
        visual_controller_->particle_size_[i] = config_.size.Random();
        auto start_color = config_.color.Random();
        visual_controller_->color_[i] = start_color;
        if (config_.fading) {
            float inv_life = 1 / life_controller_->life_[i];

            delta_color_[i] = math::Vec4 {
                -start_color[0] * inv_life,
                -start_color[1] * inv_life,
                -start_color[2] * inv_life,
                -start_color[3] * inv_life,
            };
        }

        auto px = config_.position[0].Random();
        auto py = config_.position[1].Random();

        visual_controller_->position_[i] = math::Vec2 { px, py };

        auto a = config_.angle.Random();
        auto s = config_.speed.Random();
        velocity_[i] = math::Vec2 { math::Cos(a) * s, math::Sin(a) * s };

        auto r = config_.rotation.Random();
        delta_rot_[i] = r;
    }
}

FountainSimulator::FountainSimulator(int cap)
    : pool_ { cap }
{
    pool_.AddChan({ LIFE_CONST, SIZE_CONST });
    pool_.AddChan({ POSITION_CONST, VELOCITY_CONST });
    pool_.AddChan({ COLOR_CONST, COLORDELTA_CONST });
    pool_.AddChan({ ROTATION_CONST, ROTATIONDELTA_CONST });

    // config
    config_.duration = math::MaxFloat32;
    config_.rate = float(cap) / 3;
    config_.life = Var { 3, 0.25 };
    config_.color = TwoColor { math::Vec4 { 1, 1, 1, 1 }, math::Vec4 { 1, 1, 1, 1 }, false };
    config_.fading = false;
    config_.size = Var { 8, 2 };
    config_.angle = Var { float(3.14 / 2), float(3.14 / 3) };
    config_.speed = Var { 120, 20 };
    config_.rotation = Var { 0, 1 };
    config_.gravity = -120;
}

void FountainSimulator::Initialize()
{
    pool_.Initialize();
    life_controller_->life_ = std::get<1>(pool_.GetField(LIFE_CONST));

    visual_controller_->particle_size_ = std::get<1>(pool_.GetField(SIZE_CONST));
    visual_controller_->position_ = std::get<2>(pool_.GetField(POSITION_CONST));
    velocity_ = std::get<2>(pool_.GetField(VELOCITY_CONST));
    visual_controller_->color_ = std::get<3>(pool_.GetField(COLOR_CONST));
    delta_color_ = std::get<3>(pool_.GetField(COLORDELTA_CONST));

    visual_controller_->rotation_ = std::get<1>(pool_.GetField(ROTATION_CONST));
    delta_rot_ = std::get<1>(pool_.GetField(ROTATIONDELTA_CONST));

    rate_controller_->Initialize(config_.duration, config_.rate);
}

void FountainSimulator::Simulate(float dt)
{
    // spawn new particle
    auto new_particle = rate_controller_->Rate(dt);
    if (new_particle > 0) {
        NewParticle(new_particle);
    }

    int32_t n = life_controller_->live_;

    // update old particle
    life_controller_->life_.Sub(n, dt);

    // position integrate: p' = p + v * t
    visual_controller_->position_.Integrate(n, velocity_, dt);

    // v' = v + g * t
    velocity_.Add(n, 0, config_.gravity * dt);

    // spin
    visual_controller_->rotation_.Integrate(n, delta_rot_, dt);

    // color
    visual_controller_->color_.Integrate(n, delta_color_, dt);

    // Gc
    life_controller_->GC(&pool_);
}

std::tuple<int, int> FountainSimulator::Size()
{
    return std::make_tuple(int(life_controller_->live_), pool_.GetCap());
}

void FountainSimulator::Visualize(PosTexColorVertex* buf, ITexture2D* tex)
{
    visual_controller_->Visualize(buf, tex, life_controller_->live_, config_.additive);
}

} // namespace ant2d
