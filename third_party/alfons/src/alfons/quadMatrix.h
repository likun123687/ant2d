/*
 * Based on The New Chronotext Toolkit
 * Copyright (C) 2014, Ariel Malka - All rights reserved.
 *
 * Adapted to Alfons
 * Copyright (C) 2015, Hannes Janetzek
 *
 * The following source-code is distributed under the Simplified BSD License.
 */

#pragma once

#include "alfons.h"

#include <third_party/HandmadeMath.h>

#include <vector>
#include <array>

namespace alfons {

class QuadMatrix {
public:
    union {
        hmm_mat4 m;

        struct {
            float m00, m10, m20, m30;
            float m01, m11, m21, m31;
            float m02, m12, m22, m32;
            float m03, m13, m23, m33;
        };
    };

    QuadMatrix();

    void load(const hmm_mat4& matrix);

    void setIdentity();

    void push();
    void pop();

    inline void setTranslation(const hmm_vec2& t) { setTranslation(t.X, t.Y, 0); }
    inline void setTranslation(const hmm_vec3& t) {
        setTranslation(t.X, t.Y, t.Z);
    }
    void setTranslation(float x, float y, float z = 0);

    inline void translate(const hmm_vec2& t) { translate(t.X, t.Y, 0); }
    inline void translate(const hmm_vec3& t) { translate(t.X, t.Y, t.Z); }
    void translate(float x, float y, float z = 0);

    inline void scale(float s) { scale(s, s, s); }
    void scale(float x, float y, float z = 1);

    void rotateX(float a);
    void rotateY(float a);
    void rotateZ(float a);
    void rotateXY(float sx, float sy);

    hmm_vec3 transform(float x, float y) const;

    void transformRect(const Rect& rect, Quad& out) const;

protected:
    std::vector<hmm_mat4> stack;
};
}
