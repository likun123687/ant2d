target("particle_example")
    set_kind("binary")
    add_deps("ant2d")
    add_packages("stb") 

    if is_plat("macosx") then
        add_files("main.mm")
    else
        add_files("main.cpp")
    end
    
target_end()

target("sim_fire")
    set_kind("binary")
    add_deps("ant2d")
    add_packages("stb") 

    if is_plat("macosx") then
        add_files("sim_fire.mm")
    else
        add_files("sim_fire.cpp")
    end
    
target_end()
