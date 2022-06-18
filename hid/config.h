#pragma once
#include <string>
#include <math/vector.h>

namespace ant2d {
struct WindowOptions
{
    std::string title;
    int width;
    int height;
    math::Vec4 clear;
    bool full_screen;
    bool no_title_bar;
    bool re_sizable;
};
}
