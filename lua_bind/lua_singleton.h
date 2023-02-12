#pragma once
#include <utils/singleton.h>
#include <third_party/sol/sol.hpp>

#define SharedLua \
    ::ant2d::Singleton<sol::state>()