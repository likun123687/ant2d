#pragma once
#include <gfx/bk/sokol_gfx_bk.h>
#include <math/matrix.h>

#if BK_PLATFORM_WINDOWS
#include <asset/windows/batch_system_shader.h>
#include <asset/windows/mesh_shader.h>
#elif BK_PLATFORM_OSX
#include <asset/macosx/batch_system_shader.h>
#include <asset/macosx/mesh_shader.h>
#else
#error "not support platform yet"
#endif
