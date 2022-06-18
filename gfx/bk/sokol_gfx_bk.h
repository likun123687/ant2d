#pragma once

#if BK_PLATFORM_WINDOWS
#define SOKOL_D3D11
#elif BK_PLATFORM_IOS
#define SOKOL_METAL
#elif BK_PLATFORM_ANDROID
#define SOKOL_GLES3
#elif BK_PLATFORM_OSX
#define SOKOL_METAL
#elif BK_PLATFORM_DUMMY
#define SOKOL_DUMMY_BACKEND
#else
#error "not support platform yet"
#endif

//#include "sokol_app.h"
#include "sokol_gfx.h"
//#include "sokol_glue.h"
