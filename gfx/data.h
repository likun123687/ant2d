#pragma once
namespace ant2d {

struct AABB {
    float x;
    float y;
    float width;
    float height;
};

bool OverlapAB(AABB* a, AABB* b)
{
    if (a->x < b->x + b->width && a->x + a->width > b->x
        && a->y < b->y + b->height && a->y + a->height > b->y) {
        return true;
    }
    return false;
}

} // namespace ant2d
