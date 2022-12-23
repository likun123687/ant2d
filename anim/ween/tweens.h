#pragma once
#include <cstdint>
#include <math/vector.h>
#include <gfx/color.h>
#include <anim/ween/animator.h>
#include <utils/debug.h>

namespace ant2d {
namespace ween {
    uint8_t U8Lerp(uint8_t from, uint8_t to, float f)
    {
        float v1 = float(from);
        float v2 = float(to);
        return uint8_t(v1 + (v2 - v1) * f);
    }

    uint16_t U16Lerp(uint16_t from, uint16_t to, float f)
    {
        float v1 = float(from);
        float v2 = float(to);
        return uint16_t(v1 + (v2 - v1) * f);
    }

    int IntLerp(int from, int to, float f)
    {
        return from + int(float(to - from) * f);
    }

    float F32Lerp(float from, float to, float f)
    {
        return from + (to - from) * f;
    }

    math::Vec2 Vec2Lerp(math::Vec2 from, math::Vec2 to, float f)
    {
        auto v2 = math::Vec2 {
            F32Lerp(from[0], to[0], f),
            F32Lerp(from[1], to[1], f),
        };
        return v2;
    }

    color::Float4 ColorLerp(color::Float4 from, color::Float4 to, float f)
    {
        auto c = color::Float4 {
            F32Lerp(from.r, to.r, f),
            F32Lerp(from.g, to.g, f),
            F32Lerp(from.b, to.b, f),
            F32Lerp(from.a, to.a, f),
        };
        return c;
    }

    // A float32 linear interpolation between a beginning and ending value.
    // It use the Animator as the input.
    struct F32Tween {
        Animator* am;
        float from;
        float to;

        // Animate sets the Animator that drives the Tween.
        Animator* Animate(Animator* am_)
        {
            am = am_;
            return am;
        }

        // Animator returns the Animator driving the Tween.
        Animator* GetAnimator()
        {
            if (!am->IsValid()) {
                Error("animator is unavailable");
            }

            return am;
        }

        // Range sets the beginning and ending value of the F32Tween.
        F32Tween* Range(float from_, float to_)
        {
            from = from_;
            to = to_;
            return this;
        }

        // Returns the interpolated value for the current value of the given Animator.
        float GetValue()
        {
            return F32Lerp(from, to, am->GetValue());
        }
    };

    // A f32.Vec2 linear interpolation between a beginning and ending value.
    // It use the Animator as the input.
    struct Vec2Tween {
        Animator* am;
        math::Vec2 from;
        math::Vec2 to;

        // Animate sets the Animator that drives the Tween.
        Animator* Animate(Animator* am_)
        {
            am = am_;
            return am;
        }

        // Animator returns the Animator driving the Tween.
        Animator* GetAnimator()
        {
            if (!am->IsValid()) {
                Error("animator is unavailable");
            }

            return am;
        }

        // Range sets the beginning and ending value of the F32Tween.
        Vec2Tween* Range(math::Vec2 from_, math::Vec2 to_)
        {
            from = from_;
            to = to_;
            return this;
        }

        // Returns the interpolated value for the current value of the given Animator.
        math::Vec2 GetValue()
        {
            return Vec2Lerp(from, to, am->GetValue());
        }
    };

    struct ColorTween {
        Animator* am;
        color::Float4 from;
        color::Float4 to;

        // Animate sets the Animator that drives the Tween.
        Animator* Animate(Animator* am_)
        {
            am = am_;
            return am;
        }

        // Animator returns the Animator driving the Tween.
        Animator* GetAnimator()
        {
            if (!am->IsValid()) {
                Error("animator is unavailable");
            }

            return am;
        }

        // Range sets the beginning and ending value of the F32Tween.
        ColorTween* Range(color::Float4 from_, color::Float4 to_)
        {
            from = from_;
            to = to_;
            return this;
        }

        // Returns the interpolated value for the current value of the given Animator.
        color::Float4 GetValue()
        {
            return ColorLerp(from, to, am->GetValue());
        }
    };
}
} // namespace ant2d
