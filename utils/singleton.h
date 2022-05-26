#pragma once

namespace ant2d {

template <typename T>
T& Singleton()
{
    static T kInstance;
    return kInstance;
}

} // namespace ant2d
