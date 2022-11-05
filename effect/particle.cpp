#include <effect/particle.h>

namespace ant2d {

void ParticleComp::SetSimulator(Simulator* sim)
{
    sim_ = sim;
}

Simulator* ParticleComp::GetSimulator()
{
    return sim_;
}

void ParticleComp::SetTexture(ITexture2D* tex)
{
    tex_ = tex;
}

ITexture2D* ParticleComp::GetTexture()
{
    return tex_;
}

void ParticleComp::Play()
{
    if (rate_controller_) {
        rate_controller_->Play();
    }
}

void ParticleComp::Stop()
{
    if (rate_controller_) {
        rate_controller_->Stop();
    }
}

void ParticleComp::Prewarm(float t)
{
    if (rate_controller_) {
        rate_controller_->Prewarm(t);
    }
}

void ParticleComp::SetZOrder(int16_t z)
{
    z_order_ = z;
}

uint16_t ParticleComp::GetZOrder()
{
    return z_order_;
}

int16_t ParticleComp::Z()
{
    return z_order_;
}

bool ParticleComp::Visible()
{
    if (visible_ == 0) {
        return false;
    }
    return true;
}

void ParticleComp::SetVisible(bool v)
{
    if (v) {
        visible_ = 1;
    } else {
        visible_ = 0;
    }
}

// The width and height of the particle system. We'll use it to
// make visibility test. The default value is {w:64, h:64}
void ParticleComp::SetSize(float w, float h)
{
    size_[0] = w;
    size_[1] = h;
}

std::tuple<float, float> ParticleComp::GetSize()
{
    return std::make_tuple(size_[0], size_[1]);
}

void ParticleComp::Reset()
{
}

}