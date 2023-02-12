#include <ant2d.h>
#include <iostream>

class LuaScene : public ant2d::Scene {
public:
    void OnEnter(ant2d::Game* g)
    {
        std::cout << "on enter" << std::endl;
    }

    void Update(float dt)
    {
        std::cout << "on enter " << dt << std::endl;
    }

    void OnExit()
    {
        std::cout << "on exit" << std::endl;
    }
};