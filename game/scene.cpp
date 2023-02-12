#include <game/scene.h>
#include <game/game.h>
#include <utils/debug.h>

namespace ant2d {

void SceneManager::Load(Scene* sn)
{
    auto callback = sn->GetOnLoadCallback();
    if (callback) {
        callback();
    }
}

void SceneManager::UnLoad(Scene* sn)
{
    auto callback = sn->GetUnLoadCallback();
    if (callback) {
        callback();
    }
}

void SceneManager::Setup(Game* g)
{
    g_ = g;
    if (h_scene_) {
        auto callback = h_scene_->GetOnLoadCallback();
        if (callback) {
            callback();
        }
        h_scene_->OnEnter(g);
    }
}

void SceneManager::Update(float dt)
{
    if (h_scene_) {
        h_scene_->Update(dt);
    }
}

void SceneManager::SetDefault(Scene* sn)
{
    h_scene_ = sn;
    stack_.emplace_back(sn);
}

void SceneManager::Push(Scene* sn)
{
    if (h_scene_) {
        h_scene_->OnExit();
    }
    h_scene_ = sn;
    stack_.emplace_back(sn);
    sn->OnEnter(g_);
}

SceneManager::SceneOwnType SceneManager::Pop()
{
    SceneOwnType ret_sn;
    size_t size = stack_.size();
    if (size > 0) {
#ifdef WITH_LUA_BIND
        ret_sn = stack_.back();
#else
        ret_sn = std::move(stack_.back());
#endif
        stack_.pop_back();
        ret_sn->OnExit();
#ifdef WITH_LUA_BIND
        UnLoad(ret_sn);
#else
        UnLoad(ret_sn.get());
#endif
        auto& next_sn = stack_.back();
        if (next_sn) {
#ifdef WITH_LUA_BIND
            h_scene_ = next_sn;
#else
            h_scene_ = next_sn.get();
#endif
            next_sn->OnEnter(g_);
        }
    }
    return ret_sn;
}

SceneManager::SceneOwnType SceneManager::Peek()
{
    SceneOwnType ret_sn;
    if (!stack_.empty()) {
#ifdef WITH_LUA_BIND
        ret_sn = stack_.back();
#else
        ret_sn = std::move(stack_.back());
#endif
        stack_.pop_back();
    }
    return ret_sn;
}

void SceneManager::Clear()
{
    for (size_t i = stack_.size() - 1; i >= 0; i--) {
        stack_[i]->OnExit();
    }
}
} // namespace ant2d
