
#include "math/common_func.h"
#include "gfx/render_data.h"
#include "gfx/bk_texture.h"

namespace ant2d {

// Var define a variable value between [Base-Var/2, Base+Var/2].
class Var {
private:
    float base_;
    float var_;

public:
    // Used returns whether the value is empty.
    bool Used()
    {
        return base_ != 0 || var_ != 0;
    }
    // Random returns a value between [Base-Var/2, Base+Var/2].
    float Random()
    {
        math::Random(base_ - var_ / 2, base_ + var_ / 2);
    }

    void SetBase(float base)
    {
        base_ = base;
    }

    void SetVar(float var)
    {
        var_ = var;
    }

    float GetBase()
    {
        return base_;
    }

    float GetVar()
    {
        return var_;
    }
};

// Simulator define how a particle-system works.
class Simulator {
    // Initialize the particle simulator.
    virtual void Initialize() = 0;

    // Run the simulator with delta time.
    virtual void Simulate(float dt) = 0;

    // Write the result to vertex-buffer.
    virtual void Visualize(std::vector<PosTexColorVertex>& buf, ITexture2D* tex) = 0;

    // Return the ParticleSize of the simulator.
    virtual std::tuple<int, int> Size() = 0;
    virtual ~Simulator() = default;
};

// ParticleSystem lifecycle controller.
class Controller {
    virtual void Stop() = 0;
    virtual void Play() = 0;
    virtual ~Controller() = default;
};

// Prewarm particle system
class WarmupController {
    virtual void Prewarm(float t) = 0;
    virtual float WarmTime() = 0;
    virtual ~WarmupController() = default;
};

}
