-- project
set_project("ant2d")

--最小版本
set_xmakever("2.3.1")
set_languages("cxx17")
set_warnings("all")
--set_warnings("all", "error")

add_requires("stb")

add_rules("mode.release", "mode.debug")
add_includedirs("$(projectdir)")

rule("sokol-shdc")
    set_extensions(".glsl")
    on_buildcmd_file(function (target, batchcmds, sourcefile, opt)
        import("lib.detect.find_tool")
        local sokol_shdc = assert(find_tool("sokol-shdc", {check = "-h"}), "sokol-shdc not found!")
        local slang = target:extraconf("rules", "sokol-shdc", "slang")
        local plat = target:extraconf("rules", "sokol-shdc", "plat")
        if not(slang) then
            slang = "glsl330"
        end

        if not(plat) then
            plat = "windows"
        end

        local target_dir = path.join(path.directory(sourcefile), plat)
        batchcmds:mkdir(target_dir)

        local targetfile = path.join(target_dir, path.basename(sourcefile) .. ".h")

        --target:add("includedirs", path.directory(sourcefile))

        batchcmds:vrunv(sokol_shdc.program, {"--input", sourcefile, "--output", targetfile, "--slang",slang, "--format", "sokol_impl", "--reflection"})
        batchcmds:show_progress(opt.progress, "${color.build.object}glsl %s", sourcefile)

        -- only rebuild the file if its changed since last run
        batchcmds:add_depfiles(sourcefile)
    end)
rule_end()

add_defines("ANT2D_DEBUG")
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
add_includedirs("$(projectdir)/third_party/fmt/include", {public = true})
add_includedirs("$(projectdir)/third_party/ZipIterator", {public = true})

target("ant2d")
    set_kind("static")
    add_packages("stb") 
    add_files("asset/*.glsl") --最好加在比较前面
    add_files("utils/content.cpp")
    add_files("gfx/bk/buffer.cpp")
    add_files("gfx/bk/texture.cpp")
    add_files("asset/image_data.cpp")
    add_files("asset/stb_image.cpp")
    add_files("asset/shader_utils.cpp")
    add_files("gfx/bk/shader.cpp")
    add_files("gfx/bk/res_manager.cpp")
    add_files("gfx/bk/queue.cpp")
    add_files("gfx/bk/render_context.cpp")
    add_files("gfx/bk/uniformblock.cpp")
    add_files("engi/entity.cpp")
    add_files("gfx/transform/transform.cpp")
    add_files("gfx/transform/transform_table.cpp")
    add_files("gfx/camera.cpp")
    add_files("game/scene.cpp")
    add_files("gfx/bk/bk.cpp")
    add_files("game/game.cpp")
    add_files("game/fps.cpp")
    add_files("gfx/render_system.cpp")
    add_files("gfx/gctx.cpp")
    add_files("gfx/batch_render.cpp")
    add_files("gfx/sprite/sprite.cpp")
    add_files("gfx/sprite/sprite_table.cpp")
    add_files("gfx/sprite/sprite_render_feature.cpp")
    add_files("gfx/bk_texture.cpp")
    add_files("asset/texture_manager.cpp")
    add_files("hid/hid_manager.cpp")
    add_files("math/vector.cpp")
    add_files("math/project.cpp")
    add_files("math/matrix.cpp")
    add_files("ant2d.cpp")
    add_files("third_party/fmt/src/format.cc")

    if (not has_config("with_test")) then
        add_files("asset/shdc.cpp")
        add_files("gfx/bk/sokol_gfx.cpp")
    else
        add_files("tests/mocks/*.cpp")
    end

    if is_plat("windows") then
        add_rules("sokol-shdc", {slang = "hlsl5", plat = "windows"})
    elseif is_plat("macosx", "iphoneos") then
        add_rules("sokol-shdc", {slang = "metal_macos", plat = "macosx"})
        add_files("utils/content.mm")

        if (not has_config("with_test")) then
            del_files("gfx/bk/sokol_gfx.cpp")
            add_files("gfx/bk/sokol_gfx.mm")
        end
    end
target_end()

if has_config("with_test") then
    add_requires("catch2")
    includes("tests")
end

--includes("ant2dnet/base")
--includes("ant2dnet/net")


