#include <ant2d.h>
#include <iostream>

#if BK_PLATFORM_OSX
#import <Foundation/Foundation.h>
#endif

#include <hid/sokol_app_desc.h>
#include <utils/debug.h>
#define SOKOL_IMPL
#include "sokol_app.h"
#include "sokol_glue.h"

#include <lua_bind/lua_singleton.h>
#include <lua_bind/bind.h>

ant2d::WindowOptions* ant2d_main(int argc, char* argv[])
{
    std::cout << "main enter" << std::endl;
    init_bind();
    sol::state& lua = SharedLua;
    std::string full_path = SharedContent.GetFullPath("asset/lua/run.lua");

    ant2d::WindowOptions* options = lua.script_file(full_path);
    load_global();
    return options;
}
