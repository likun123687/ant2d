#include <effect/life_controller.h>
namespace ant2d {

int LifeController::GC(Pool* p)
{
    int i = 0;
    int j = live_ - 1;
    while (i <= j) {
        if (life_[i] < 0) {
            p->Swap(i, j);
            j--;
        } else {
            i++;
        }
    }

    int dead = live_ - i;
    live_ = i;
    return dead;
}
} // namespace ant2d
