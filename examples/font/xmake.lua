target("font_example")
    set_kind("binary")
    add_deps("ant2d")
    add_packages("stb") 

    if is_plat("macosx") then
        add_files("main.mm")
    else
        add_files("main.cpp")
    end
    
target_end()

