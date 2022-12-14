#include <math/vector.h>

namespace ant2d {
using FingerId = int;
//// Touch & Mouse input go here
struct PointerInput {
    // The mouse pointer always has a pointer-id of 0
    FingerId id;

    // The position and moved amount of pointer
    math::Vec2 mouse_pos;
    math::Vec2 mouse_delta;
    bool used;
};
}
