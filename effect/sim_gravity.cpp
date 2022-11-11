#include <effect/sim_gravity.h>

namespace ant2d {
void GravitySimulator::NewParticle(int new_particle)
{
    if (life_controller_->live_ + new_particle > pool_.GetCap()) {
        return;
    }

    auto start = life_controller_->live_;
    life_controller_->live_ += new_particle;

    for (int i = start; i < life_controller_->live_; i++) {
        life_controller_->life_[i] = config_.life.Random();
        float inv_life = 1 / life_controller_->life_[i];

        visual_controller_->position_[i] = math::Vec2 { config_.life.GetBase(), config_.life.GetBase() + config_.life.GetVar() };
        // Color
        float red = 0, g = 0, b = 0, a = 1;
        float redd, gd, bd, ad;

        if (config_.r.Used()) {
            std::tie(red, redd) = config_.r.RangeInit(inv_life);
        }

        if (config_.g.Used()) {
            std::tie(g, gd) = config_.g.RangeInit(inv_life);
        }

        if (config_.b.Used()) {
            std::tie(b, bd) = config_.b.RangeInit(inv_life);
        }

        if (config_.a.Used()) {
            std::tie(a, ad) = config_.a.RangeInit(inv_life);
        }

        visual_controller_->color_[i] = math::Vec4 { red, g, b, a };
        color_delta_[i] = math::Vec4 { redd, gd, bd, ad };

        std::tie(visual_controller_->particle_size_[i], size_delta_[i]) = config_.size.RangeInit(inv_life);

        // rot
        std::tie(visual_controller_->rotation_[i], rot_delta_[i]) = config_.rot.RangeInit(inv_life);

        // start position
        pose_start_[i] = visual_controller_->position_[i];

        // gravity
        radial_acc_[i] = config_.radial_acc.Random();
        tangential_acc_[i] = config_.tangential_acc.Random();

        auto angel = config_.angel.Random();
        auto speed = config_.speed.Random();
        velocity_[i] = math::Vec2 { math::Cos(angel) * speed, math::Sin(angel) * speed };
    }
}

GravitySimulator::GravitySimulator(GravityConfig cfg)
    : config_ { cfg }
    , pool_ { cfg.max }
{
    pool_.AddChan({ LIFE_CONST });
    pool_.AddChan({ POSITION_CONST, POSITIONSTART_CONST });
    pool_.AddChan({ COLOR_CONST, COLORDELTA_CONST });
    pool_.AddChan({ SIZE_CONST, SIZEDELTA_CONST });
    pool_.AddChan({ ROTATION_CONST, ROTATIONDELTA_CONST });

    pool_.AddChan({ VELOCITY_CONST });
    pool_.AddChan({ RADIALACC_CONST });
    pool_.AddChan({ TANGENTIALACC_CONST });
}

void GravitySimulator::Initialize()
{
    pool_.Initialize();

    life_controller_->life_ = std::get<1>(pool_.GetField(LIFE_CONST));
    visual_controller_->position_ = std::get<2>(pool_.GetField(POSITION_CONST));
    pose_start_ = std::get<2>(pool_.GetField(POSITIONSTART_CONST));
    visual_controller_->color_ = std::get<3>(pool_.GetField(COLOR_CONST));
    color_delta_ = std::get<3>(pool_.GetField(COLORDELTA_CONST));
    visual_controller_->particle_size_ = std::get<1>(pool_.GetField(SIZE_CONST));
    size_delta_ = std::get<1>(pool_.GetField(SIZEDELTA_CONST));
    visual_controller_->rotation_ = std::get<1>(pool_.GetField(ROTATION_CONST));
    rot_delta_ = std::get<1>(pool_.GetField(ROTATION_CONST));
    velocity_ = std::get<2>(pool_.GetField(VELOCITY_CONST));
    radial_acc_ = std::get<1>(pool_.GetField(RADIALACC_CONST));
    tangential_acc_ = std::get<1>(pool_.GetField(TANGENTIALACC_CONST));

    // init const
    gravity_ = config_.gravity;

    // emit rate
    rate_controller_->Initialize(config_.duration, config_.rate);
}

void GravitySimulator::Simulate(float dt)
{
    // spawn new particle
    auto new_particle = rate_controller_->Rate(dt);
    if (new_particle > 0) {
        NewParticle(new_particle);
    }

    int32_t n = life_controller_->live_;

    life_controller_->life_.Sub(n, dt);

    // gravity model integrate
    velocity_.radialIntegrate(n, visual_controller_->position_, radial_acc_, dt);
    velocity_.tangentIntegrate(n, visual_controller_->position_, tangential_acc_, dt);

    velocity_.Add(n, gravity_[0] * dt, gravity_[1] * dt);

    // position
    visual_controller_->position_.Integrate(n, velocity_, dt);

    // Color
    visual_controller_->color_.Integrate(n, color_delta_, dt);

    // ParticleSize
    visual_controller_->particle_size_.Integrate(n, size_delta_, dt);

    // angle
    visual_controller_->rotation_.Integrate(n, rot_delta_, dt);

    // recycle dead
    life_controller_->GC(&pool_);
}

std::tuple<int, int> GravitySimulator::Size()
{
    return std::make_tuple(int(life_controller_->live_), pool_.GetCap());
}

void GravitySimulator::Visualize(PosTexColorVertex* buf, ITexture2D* tex)
{
    visual_controller_->Visualize(buf, tex, life_controller_->live_, config_.additive);
}

} // namespace ant2d
