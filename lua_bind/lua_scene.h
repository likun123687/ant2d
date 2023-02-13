#include <ant2d.h>
#include <iostream>
#include <lua_bind/lua_singleton.h>

class LuaScene : public ant2d::Scene {
public:
    virtual void OnEnter(ant2d::Game* g) override
    {
        if (on_enter_cb_) {
            auto result = on_enter_cb_(this, g);
            if (!result.valid()) {
                sol::error err = result;
                Error("on enter cb error {}", err.what());
            }
            return;
        }

        std::cout << "cpp on enter" << std::endl;
    }

    virtual void Update(float dt) override
    {
        if (update_cb_) {
            auto result = update_cb_(this, dt);
            if (!result.valid()) {
                sol::error err = result;
                Error("update cb error {}", err.what());
            }
            return;
        }
        std::cout << "cpp update " << dt << std::endl;
    }

    virtual void OnExit() override
    {
        if (on_exit_cb_) {
            auto result = on_exit_cb_(this);
            if (!result.valid()) {
                sol::error err = result;
                Error("on exit cb error {}", err.what());
            }
            return;
        }
        std::cout << "cpp on exit" << std::endl;
    }

    void Setter(sol::object key, sol::object value, sol::this_state)
    {
        const std::string& key_str = key.as<std::string>();
        if (key_str == "OnEnterCb") {
            on_enter_cb_ = value.as<sol::protected_function>();
        } else if (key_str == "UpdateCb") {
            update_cb_ = value.as<sol::protected_function>();
        } else if (key_str == "OnExitCb") {
            on_exit_cb_ = value.as<sol::protected_function>();
        }
    }

private:
    sol::protected_function on_enter_cb_;
    sol::protected_function update_cb_;
    sol::protected_function on_exit_cb_;
};