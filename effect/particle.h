#pragma once
#include <gfx/i_comp.h>
#include <effect/common.h>

namespace ant2d {
class ParticleComp : public IComp {
private:
    bool init_;
    Simulator* sim_;
    Controller* ctrl_;
    WarmupController* warmup_ctrl_;
    uint16_t z_order_;
    uint16_t visible_;

    ITexture2D* tex_;
    math::Vec2 size_;

public:
    void SetSimulator(Simulator* sim);

    Simulator* GetSimulator();

    void SetTexture(ITexture2D* tex);

    ITexture2D* GetTexture();

    void Play();

    void Stop();

    void Prewarm(float t);

    void SetZOrder(int16_t z);

    int16_t Z();

    bool Visible();

    void SetVisible(v bool);
    // The width and height of the particle system. We'll use it to
    // make visibility test. The default value is {w:64, h:64}
    void SetSize(float w, float h);
};
}