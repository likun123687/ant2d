target("ant2d_test")
    set_kind("binary")
    add_deps("ant2d_common", "mock")
    add_packages("catch2") 
    add_packages("stb") 
    add_files("ant2d_test.cpp")
    add_files("camera_test.cpp")
    add_files("game_test.cpp")
    add_files("bk_test.cpp")
    add_files("batch_render_test.cpp")
    add_files("texture_manager_test.cpp")
    add_files("effect_test.cpp")
    
target_end()

