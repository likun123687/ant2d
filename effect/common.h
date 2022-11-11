
#pragma once
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
    Var(float base, float var)
        : base_ { base }
        , var_ { var }
    {
    }
    Var()
        : base_ { 0 }
        , var_ { 0 }
    {
    }
    // Used returns whether the value is empty.
    bool Used()
    {
        return base_ != 0 || var_ != 0;
    }
    // Random returns a value between [Base-Var/2, Base+Var/2].
    float Random()
    {
        return math::Random(base_ - var_ / 2, base_ + var_ / 2);
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

    friend bool operator!=(const Var& v1, const Var& v2);
};

inline bool operator!=(const Var& v1, const Var& v2)
{
    return v1.base_ != v2.base_ || v1.var_ != v2.var_;
}

// Range define a range between [Start, End].
class Range {
private:
    Var start_;
    Var end_;

public:
    Range()
        : start_ {}
        , end_ {}
    {
    }

    Range(Var start, Var end)
        : start_ { start }
        , end_ { end }
    {
    }
    // Used returns whether the value is empty.
    bool Used()
    {
        return start_.Used() || end_.Used();
    }

    // HasRange returns whether Start and End is the same.
    bool HasRange()
    {
        return start_ != end_;
    }

    std::tuple<float, float> RangeInit(float inv_life)
    {
        auto start = start_.Random();
        float d = .0f;
        if (start_ != end_) {
            d = (end_.Random() - start) * inv_life;
        }
        return std::make_tuple(start, d);
    }

    Var& GetStart()
    {
        return start_;
    }

    Var& GetEnd()
    {
        return end_;
    }
};

class TwoColor {
private:
    math::Vec4 one_;
    math::Vec4 other_;
    bool enable_gradient_;

public:
    TwoColor()
        : one_ {}
        , other_ {}
        , enable_gradient_ { false }
    {
    }

    TwoColor(math::Vec4 one, math::Vec4 other, bool enable_gradient)
        : one_ { one }
        , other_ { other }
        , enable_gradient_ { enable_gradient }
    {
    }

    math::Vec4 Random()
    {
        math::Vec4 c {};
        auto f = math::Random(0, 1);
        if (enable_gradient_) {
            c[0] = one_[0] + (other_[0] - one_[0]) * f;
            c[1] = one_[1] + (other_[1] - one_[1]) * f;
            c[2] = one_[2] + (other_[2] - one_[2]) * f;
            c[3] = one_[3] + (other_[3] - one_[3]) * f;
        } else {
            if (f > 0.5) {
                c = other_;
            } else {
                c = one_;
            }
        }
        return c;
    }
};

// ParticleSystem lifecycle controller.
class IController {
public:
    virtual void Stop() = 0;
    virtual void Play() = 0;
    virtual void Prewarm(float t) = 0;
    virtual float WarmTime() = 0;
    virtual ~IController() = default;
};


// Simulator define how a particle-system works.
class ISimulator {
public:
    // Initialize the particle simulator.
    virtual void Initialize() = 0;

    // Run the simulator with delta time.
    virtual void Simulate(float dt) = 0;

    // Write the result to vertex-buffer.
    virtual void Visualize(PosTexColorVertex *buf, ITexture2D* tex) = 0;

    // Return the ParticleSize of the simulator.
    virtual std::tuple<int, int> Size() = 0;

    virtual IController* GetController() = 0;

    virtual ~ISimulator() = default;
};


}
