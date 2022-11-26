#pragma once
#include <gfx/i_comp.h>
#include <effect/common.h>
#include <effect/rate_controller.h>

namespace ant2d {
class ParticleComp : public IComp {
private:
    bool is_init_;
    std::unique_ptr<ISimulator> sim_;
    // RateController* rate_controller_;
    uint16_t z_order_;
    uint16_t visible_;

    ITexture2D* tex_;
    math::Vec2 size_;

public:
    void SetSimulator(ISimulator* sim);

    ISimulator* GetSimulator();

    void SetTexture(ITexture2D* tex);

    ITexture2D* GetTexture();

    void Play();

    void Stop();

    void Prewarm(float t);

    void SetZOrder(int16_t z);
    uint16_t GetZOrder();

    int16_t Z();

    bool Visible();

    void SetVisible(bool v);
    // The width and height of the particle system. We'll use it to
    // make visibility test. The default value is {w:64, h:64}
    void SetSize(float w, float h);
    std::tuple<float, float> GetSize();
    void Reset();

    bool GetIsInit()
    {
        return is_init_;
    }

    void SetIsInit(bool is_init)
    {
        is_init_ = is_init;
    }

    void Init()
    {
        if (sim_) {
            sim_->Initialize();
        }

        auto controller = sim_->GetController();
        if (controller && controller->WarmTime() > 0) {
            Warmup(sim_.get(), controller->WarmTime());
        }
    }

    static void Warmup(ISimulator* sim, float t)
    {
        for (float dt = 1.0f / 30; t > 0; t -= dt) {
            sim->Simulate(dt);
        }
    }
};
}