/*
 * Based on The New Chronotext Toolkit
 * Copyright (C) 2014, Ariel Malka - All rights reserved.
 *
 * Adapted to Alfons
 * Copyright (C) 2015, Hannes Janetzek
 *
 * The following source-code is distributed under the Simplified BSD License.
 */

/*
 * References:
 *
 * "Spline Curves" BY Tim Lambert
 * http://www.cse.unsw.edu.au/~lambert/splines
 */

/*
 * TODO:
 *
 * Implementing the hermite curve (with bias and tension) would be a must:
 * http://paulbourke.net/miscellaneous/interpolation
 */

#pragma once

#include "third_party/HandmadeMath.h"

#include <vector>

namespace alfons {

enum class SplineType {
    bspline,
    catmull_rom
};

class SplinePath {
public:

    SplinePath(int capacity = 0);
    SplinePath(const std::vector<hmm_vec2>& points);
    void add(const std::vector<hmm_vec2>& points);
    void add(const hmm_vec2& point);
    inline void add(float x, float y) { add(HMM_Vec2(x, y)); }

    const std::vector<hmm_vec2>& getPoints() const;

    void clear();
    int size() const;
    bool empty() const;

    void close();
    bool isClosed() const;

    void flush(SplineType type, std::vector<hmm_vec2>& path, float tol = 1) const;

protected:
    std::vector<hmm_vec2> points;
    bool closed;
};

static inline hmm_vec2 GammaBSpline(float t, hmm_vec2* in) {
    float w0 = ((3 - t) * t - 3) * t + 1;
    float w1 = ((3 * t - 6) * t) * t + 4;
    float w2 = ((3 - 3 * t) * t + 3) * t + 1;
    float w3 = t * t * t;

    return HMM_Vec2(w0 * in[0].X + w1 * in[1].X + w2 * in[2].X + w3 * in[3].X,
                     w0 * in[0].Y + w1 * in[1].Y + w2 * in[2].Y + w3 * in[3].Y) /
           6.0f;
}

static inline hmm_vec2 GammaCatmullRom(float t, hmm_vec2* in) {
    float w0 = ((2 - t) * t - 1) * t;
    float w1 = ((3 * t - 5) * t) * t + 2;
    float w2 = ((4 - 3 * t) * t + 1) * t;
    float w3 = (t - 1) * t * t;

    return HMM_Vec2(w0 * in[0].X + w1 * in[1].X + w2 * in[2].X + w3 * in[3].X,
                     w0 * in[0].Y + w1 * in[1].Y + w2 * in[2].Y + w3 * in[3].Y) /
           2.0f;
}
}
