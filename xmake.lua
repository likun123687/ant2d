-- project
set_project("ant2d")

--最小版本
set_xmakever("2.3.1")
set_languages("cxx17")

add_rules("mode.release", "mode.debug") 
add_includedirs("$(projectdir)")

if is_plat("windows") then
    add_cxflags("/utf-8")
    add_defines("BK_PLATFORM_WINDOWS")
elseif is_plat("macosx") then
    add_defines("BK_PLATFORM_OSX")
    add_frameworks("Foundation")
    add_frameworks("Cocoa")
    add_frameworks("QuartzCore")
    add_frameworks("Metal")
    add_frameworks("MetalKit")
end

option("with_test")
    set_default(false)
    set_showmenu(true)
    set_description("build with ant2d test")
option_end()

add_includedirs("$(projectdir)/third_party/cpp-stdio-file-wrapper/include",  {public = true})
add_includedirs("$(projectdir)/third_party/sokol", {public = true})
add_includedirs("$(projectdir)/third_party/trompeloeil/include", {public = true})

target("ant2d")
    set_kind("static")
    add_files("utils/*.cpp")
    add_files("utils/silly/*.cpp")
    add_files("gfx/bk/buffer.cpp")
    add_files("gfx/bk/sokol_gfx.cpp")
    if is_plat("macosx", "iphoneos") then
        del_files("gfx/bk/sokol_gfx.cpp")
        add_files("gfx/bk/sokol_gfx.mm")
        add_files("utils/*.mm")
    end
target_end()

if has_config("with_test") then
    add_requires("catch2")
    includes("tests")
end

--includes("ant2dnet/base")
--includes("ant2dnet/net")


