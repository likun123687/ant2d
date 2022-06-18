

using FingerId = int;
//// Touch & Mouse input go here
class PointerInput
{
    // The mouse pointer always has a pointer-id of 0
    FingerId id_;

    // The position and moved amount of pointer
    math::Vec2 mouse_pos_;
    math::Vec2 mouse_delta_;
    boll used_;
}

