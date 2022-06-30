#include <game/scene.h>
#include <game/game.h>
#include <utils/debug.h>

namespace ant2d {

void SceneManager::Load(Scene *sn)
{
    auto callback = sn->GetOnLoadCallback();
    if (callback) {
        callback();
    }
}

void SceneManager::UnLoad(Scene *sn)
{
    auto callback = sn->GetUnLoadCallback();
    if (callback) {
        callback();
    }
}

void SceneManager::Setup(Game *g)
{
    Info("g {}", static_cast<void *>(g));
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

void SceneManager::SetDefault(Scene *sn)
{
    h_scene_ = sn;
    stack_.emplace_back(sn);
}

void SceneManager::Push(Scene *sn)
{
    if (h_scene_) {
        h_scene_->OnExit();
    }
    h_scene_ = sn;
    stack_.emplace_back(sn);
    sn->OnEnter(g_);
}

std::unique_ptr<Scene> SceneManager::Pop()
{
    std::unique_ptr<Scene> ret_sn;
    size_t size =  stack_.size();
    if (size > 0) {
        ret_sn = std::move(stack_.back());
        stack_.pop_back();
        ret_sn->OnExit();
        UnLoad(ret_sn.get());

        auto &next_sn = stack_.back();
        if (next_sn) {
            h_scene_ = next_sn.get();
            next_sn->OnExit();
        }
    }
    return ret_sn;
}

std::unique_ptr<Scene> SceneManager::Peek()
{
    std::unique_ptr<Scene> ret_sn;
    if (!stack_.empty()) {
        ret_sn = std::move(stack_.back());
        stack_.pop_back();
    }
    return ret_sn;
}

void SceneManager::Clear()
{
    for (int i = stack_.size() - 1; i>=0; i--) {
        stack_[i]->OnExit();
    }
}
}//namespace ant2d
