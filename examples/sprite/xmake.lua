target("sprite_example")
    set_kind("binary")
    add_deps("ant2d")
    add_packages("stb") 

    if is_plat("macosx") then
        add_files("main.mm")
    else
        add_files("main.cpp")
    end
    after_build(function (target)
        os.cp("tests/assets/", path.join(target:targetdir(), "assets"), {rootdir = "tests/assets/"})
        print("copy test assets to target dir success\n")
    end)
target_end()

