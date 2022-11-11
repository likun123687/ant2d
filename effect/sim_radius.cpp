#include <effect/sim_radius.h>

namespace ant2d {
void RadiusSimulator::NewParticle(int new_particle)
{
    if (life_controller_->live_ + new_particle > pool_.GetCap()) {
        return;
    }

    auto start = life_controller_->live_;
    life_controller_->live_ += new_particle;

    for (int i = start; i < life_controller_->live_; i++) {
        life_controller_->life_[i] = config_.life.Random();
        float inv_life = 1 / life_controller_->life_[i];

        visual_controller_->position_[i] = math::Vec2 { config_.x.Random(), config_.y.Random() };
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

        visual_controller_->particle_size_[i] = config_.size.GetStart().Random();
        if (config_.size.GetStart() != config_.size.GetEnd()) {
            size_delta_[i] = (config_.size.GetEnd().Random() - visual_controller_->particle_size_[i]) * inv_life;
        }

        // rot
        visual_controller_->rotation_[i] = config_.rot.GetStart().Random();
        if (config_.rot.GetStart() != config_.rot.GetEnd()) {
            rot_delta_[i] = (config_.rot.GetEnd().Random() - visual_controller_->rotation_[i]) * inv_life;
        }

        // start position
        pose_start_[i] = visual_controller_->position_[i];

        // radius
        radius_[i] = config_.radius.GetStart().Random();
        if (config_.radius.GetStart() != config_.radius.GetEnd()) {
            radius_delta_[i] = (config_.radius.GetEnd().Random() - visual_controller_->rotation_[i]) * inv_life;
        }

        // angle
        angle_[i] = config_.angle.Random();
        angle_delta_[i] = config_.angle_delta.Random();
    }
}

RadiusSimulator::RadiusSimulator(RadiusConfig cfg)
    : config_ { cfg }
    , pool_ { cfg.max }
{
    pool_.AddChan({ LIFE_CONST });
    pool_.AddChan({ POSITION_CONST, POSITIONSTART_CONST });
    pool_.AddChan({ COLOR_CONST, COLORDELTA_CONST });
    pool_.AddChan({ SIZE_CONST, SIZEDELTA_CONST });
    pool_.AddChan({ ROTATION_CONST, ROTATIONDELTA_CONST });

    pool_.AddChan({ ANGLE_CONST, ANGLEDELTA_CONST });
    pool_.AddChan({ RADIUS_CONST, RADIUSDELTA_CONST });
}

void RadiusSimulator::Initialize()
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
    angle_ = std::get<1>(pool_.GetField(ANGLE_CONST));
    angle_delta_ = std::get<1>(pool_.GetField(ANGLEDELTA_CONST));
    radius_ = std::get<1>(pool_.GetField(RADIUS_CONST));
    radius_delta_ = std::get<1>(pool_.GetField(RADIUSDELTA_CONST));

    // init controller
    rate_controller_->Initialize(config_.duration, config_.rate);
}

void RadiusSimulator::Simulate(float dt)
{
    // spawn new particle
    auto new_particle = rate_controller_->Rate(dt);
    if (new_particle > 0) {
        NewParticle(new_particle);
    }

    int32_t n = life_controller_->live_;

    life_controller_->life_.Sub(n, dt);
    angle_.Integrate(n, angle_delta_, dt);
    radius_.Integrate(n, radius_delta_, dt);
    for (int i = 0; i < n; i++) {
        auto x = float(math::Cos(angle_[i])) * radius_[i];
        auto y = float(math::Sin(angle_[i])) * radius_[i];
        visual_controller_->position_[i] = math::Vec2 { x, y };
    }

    // Color
    visual_controller_->color_.Integrate(n, color_delta_, dt);

    // ParticleSize
    visual_controller_->particle_size_.Integrate(n, size_delta_, dt);

    // angle
    visual_controller_->rotation_.Integrate(n, rot_delta_, dt);

    // recycle dead
    life_controller_->GC(&pool_);
}

std::tuple<int, int> RadiusSimulator::Size()
{
    return std::make_tuple(int(life_controller_->live_), pool_.GetCap());
}

void RadiusSimulator::Visualize(PosTexColorVertex* buf, ITexture2D* tex)
{
    visual_controller_->Visualize(buf, tex, life_controller_->live_, config_.additive);
}

} // namespace ant2d
