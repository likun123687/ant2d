#pragma once

#include <cassert>
#include <string>
#include <fmt/core.h>
#include <cstdlib>

namespace ant2d {
#define ANT2D_DUMMY do {} while (0)

#define assertm(exp, msg) assert(((void)msg, exp))

#if !ANT2D_DEBUG
    #define Info(...) ANT2D_DUMMY
    #define Warn(...) ANT2D_DUMMY
    #define Error(...) ANT2D_DUMMY
#else
    #define Info(format, ...)  \
        fmt::print("[Ant2d Info] [File] {}, [Func] {}, [Line] {}, [Message] " format "\n",\
                __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
    #define Warn(format, ...)  \
        fmt::print("[Ant2d Warn] [File] {}, [Func] {}, [Line] {}, [Message] " format "\n",\
                __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
    #define Error(format, ...)  \
        fmt::print("[Ant2d Error] [File] {}, [Func] {}, [Line] {}, [Message] " format "\n",\
                __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);abort()
#endif
}

