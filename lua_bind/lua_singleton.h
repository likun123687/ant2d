#pragma once
#include <utils/singleton.h>
#define SOL_ALL_SAFETIES_ON 1
#include <third_party/sol/sol.hpp>

#define SharedLua \
    ::ant2d::Singleton<sol::state>()