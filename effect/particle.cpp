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
    if (ctrl_) {
        ctrl_->Play();
    }
}

void ParticleComp::Stop()
{
    if (ctrl_) {
        ctrl_->Stop();
    }
}

void ParticleComp::Prewarm(float t)
{
    if (warmup_ctrl_) {
        warmup_ctrl_->Prewarm(t);
    }
}

void ParticleComp::SetZOrder(int16_t z)
{
    z_order_ = z;
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

void ParticleComp::SetVisible(v bool)
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
}