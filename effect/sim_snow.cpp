#include <effect/sim_snow.h>

namespace ant2d {
void SnowSimulator::NewParticle(int new_particle)
{
    if (life_controller_->live_ + new_particle > pool_.GetCap()) {
        return;
    }

    auto start = life_controller_->live_;
    life_controller_->live_ += new_particle;

    for (int i = start; i < life_controller_->live_; i++) {
        life_controller_->life_[i] = config_.life.Random();
        visual_controller_->color_[i] = config_.color;
        visual_controller_->particle_size_[i] = config_.size.Random();

        auto f = visual_controller_->particle_size_[i] / (config_.size.GetBase() + config_.size.GetVar());
        visual_controller_->color_[i][3] = f;
        visual_controller_->rotation_[i] = config_.rotation.Random();

        auto px = config_.position[0].Random();
        auto py = config_.position[1].Random();

        visual_controller_->position_[i] = math::Vec2 { px, py };

        auto dx = config_.velocity[0].Random();
        auto dy = config_.velocity[1].Random();
        velocity_[i] = math::Vec2 { dx, dy };
    }
}

SnowSimulator::SnowSimulator(int cap, float w, float h)
    : pool_ { cap }
{
    pool_.AddChan({ LIFE_CONST, SIZE_CONST });
    pool_.AddChan({ POSITION_CONST, VELOCITY_CONST });
    pool_.AddChan({ COLOR_CONST });
    pool_.AddChan({ ROTATION_CONST });
    pool_.AddChan({ ROTATIONDELTA_CONST });

    config_.duration = math::MaxFloat32;
    config_.rate = float(cap) / 10;
    config_.life = Var { 10, 4 };
    config_.color = math::Vec4 { 1, 1, 1, 1 };
    config_.size = Var { 6, 6 };
    config_.position[0] = Var { 0, w };
    config_.position[1] = Var { h / 2, 0 };
    config_.velocity[0] = Var { -10, 20 };
    config_.velocity[1] = Var { -50, 20 };
    config_.rotation = Var { 0, float(3.14 / 180) };
}

void SnowSimulator::Initialize()
{
    pool_.Initialize();

    life_controller_->life_ = std::get<1>(pool_.GetField(LIFE_CONST));
    visual_controller_->particle_size_ = std::get<1>(pool_.GetField(SIZE_CONST));
    visual_controller_->position_ = std::get<2>(pool_.GetField(POSITION_CONST));
    velocity_ = std::get<2>(pool_.GetField(VELOCITY_CONST));
    visual_controller_->color_ = std::get<3>(pool_.GetField(COLOR_CONST));
    visual_controller_->rotation_ = std::get<1>(pool_.GetField(ROTATION_CONST));
    delta_rot_ = std::get<1>(pool_.GetField(ROTATIONDELTA_CONST));

    // init controller
    rate_controller_->Initialize(config_.duration, config_.rate);
}

void SnowSimulator::Simulate(float dt)
{
    // spawn new particle
    auto new_particle = rate_controller_->Rate(dt);
    if (new_particle > 0) {
        NewParticle(new_particle);
    }

    int32_t n = life_controller_->live_;

    life_controller_->life_.Sub(n, dt);
    visual_controller_->position_.Integrate(n, velocity_, dt);
    visual_controller_->rotation_.Integrate(n, delta_rot_, dt);

    // recycle dead
    life_controller_->GC(&pool_);
}

std::tuple<int, int> SnowSimulator::Size()
{
    return std::make_tuple(int(life_controller_->live_), pool_.GetCap());
}

void SnowSimulator::Visualize(PosTexColorVertex* buf, ITexture2D* tex)
{
    visual_controller_->Visualize(buf, tex, life_controller_->live_, false);
}

} // namespace ant2d
