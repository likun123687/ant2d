#pragma once

#include <functional>
#include <vector>

namespace ant2d {

class Game;

class Scene {
    using Callback = std::function<void()>;

private:
    Callback on_load_callback_;
    Callback un_load_callback_;

public:
    virtual void OnEnter(Game* g) = 0;
    virtual void Update(float dt) = 0;
    virtual void OnExit() = 0;
    Callback GetOnLoadCallback()
    {
        return on_load_callback_;
    }

    void SetOnLoadCallback(Callback callback)
    {
        on_load_callback_ = callback;
    }

    Callback GetUnLoadCallback()
    {
        return un_load_callback_;
    }

    void SetUnLoadCallback(Callback callback)
    {
        un_load_callback_ = callback;
    }

    virtual ~Scene() = default;
};

class SceneManager {
public:
#ifdef WITH_LUA_BIND
    using SceneOwnType = Scene*;
#else
    using SceneOwnType = std::unique_ptr<Scene>;
#endif

    void Load(Scene* sn);
    void UnLoad(Scene* sn);
    void Setup(Game* g);
    void Update(float dt);
    void SetDefault(Scene* sn);
    void Push(Scene* sn);
    SceneOwnType Pop();
    SceneOwnType Peek();
    void Clear();

private:
    Game* g_;
    std::vector<SceneOwnType> stack_;
    Scene* h_scene_;
};
}
