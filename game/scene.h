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
    void Load(Scene* sn);
    void UnLoad(Scene* sn);
    void Setup(Game* g);
    void Update(float dt);
    void SetDefault(Scene* sn);
    void Push(Scene* sn);
    std::unique_ptr<Scene> Pop();
    std::unique_ptr<Scene> Peek();
    void Clear();

private:
    Game* g_;
    std::vector<std::unique_ptr<Scene>> stack_;
    Scene* h_scene_;
};
}
