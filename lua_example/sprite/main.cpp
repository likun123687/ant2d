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

    // std::string full_path = SharedContent.GetFullPath("assets/lua/run.lua");
    sol::protected_function_result result = lua.script_file("assets/lua/run.lua", [](lua_State*, sol::protected_function_result pfr) {
        return pfr;
    });
    if (!result.valid()) {
        sol::error err = result;
        std::cout << err.what() << std::endl;
    }
    load_global();
    ant2d::WindowOptions* options = result;
    return options;
}
