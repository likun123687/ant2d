/*
 * From:
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * REFERENCES:
 *
 * "Adaptive Sampling of Parametric Curves" BY Luiz Henrique de Figueiredo
 * http://ariel.chronotext.org/dd/defigueiredo93adaptive.pdf
 */

#pragma once

#include "third_party/HandmadeMath.h"

#include <vector>
#include <array>
#include <functional>
#include <cstdlib>

namespace alfons {

class ASPC {
public:
    ASPC(std::function<hmm_vec2(float, hmm_vec2*)> gamma, std::vector<hmm_vec2>& path, float tol = 1)
        : gamma(gamma)
        , path(path)
        , tol(tol)
    {
        /*
         * The original algorithm requires randomness, but we want to avoid the noise
         * introduced by the same randomness when successively sampling the same points
         *
         * Requirements:
         * - Multiple ASPC instances should not be processed concurently
         * - Sampling should take place right after aspc is created
         */
        srand(1);
    }

    void segment(const hmm_vec2& p0, const hmm_vec2& p1, const hmm_vec2& p2, const hmm_vec2& p3)
    {
        in[0] = p0;
        in[1] = p1;
        in[2] = p2;
        in[3] = p3;

        float pt = 0;
        auto p = gamma(pt, in.data());

        float qt = 1;
        auto q = gamma(qt, in.data());

        sample(pt, p, qt, q);
    }

protected:
    std::function<hmm_vec2(float, hmm_vec2*)> gamma;
    std::vector<hmm_vec2>& path;
    float tol;

    std::array<hmm_vec2, 4> in;

    void sample(float t0, const hmm_vec2& p0, float t1, const hmm_vec2& p1)
    {
        float t = 0.45f + 0.1f * rand() / RAND_MAX;
        float rt = t0 + t * (t1 - t0);
        auto r = gamma(rt, in.data());

        // float cross = (p0 - r).cross(p1 - r);
        // float cross = glm::dot(hmm_vec3(p0 - r, 0.0f), hmm_vec3(p1 - r, 0.0f));
        auto pr0 = p0 - r;
        auto pr1 = p1 - r;
        float cross = HMM_Dot(HMM_Vec3(pr0.X, pr0.Y, 0.0f), HMM_Vec3(pr1.X, pr1.Y, 0.0f));

        if (cross * cross < tol) {
            path.push_back(p0);
        } else {
            sample(t0, p0, rt, r);
            sample(rt, r, t1, p1);
        }
    }
};
}
