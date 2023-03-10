#pragma once

#include <engi/entity.h>
#include <game/fps.h>
#include <engi/base_table.h>
#include <game/scene.h>
#include <gfx/render_system.h>
#include <hid/life_cycle.h>
#include <effect/particle_simulate_system.h>
#include <hid/input/input_system.h>
#include <anim/anim.h>
#include <game/script/script_system.h>

namespace ant2d {
constexpr int32_t kMaxScriptSize = 1024;
constexpr int32_t kMaxSpriteSize = 64 << 10;
constexpr int32_t kMaxTransformSize = 64 << 10;
constexpr int32_t kMaxTextSize = 64 << 10;
constexpr int32_t kMaxMeshSize = 64 << 10;
constexpr int32_t kMaxParticleSize = 1024;

struct Options {
    int32_t width;
    int32_t height;
};

class DB {
private:
    std::unique_ptr<EntityManager> entity_manager_;
    TableList tables_;

public:
    void AddTable(IBase* table);
    TableList& GetTableList();
    EntityManager* GetEntityManager();
    void SetEntityManager(EntityManager* entity_manager);
};

class AppState {
    using StateChangeCallback = std::function<void(bool)>;
    struct State {
        bool paused;
        bool lost_focus;
    };

private:
    StateChangeCallback pause_callback_;
    StateChangeCallback focus_callback_;
    State old_;
    State now_;

public:
    void SetPaused(bool paused);
    void SetFocused(bool focused);
    StateChangeCallback GetPauseCallback();
    StateChangeCallback GetFocusCallback();
};

class Game : public WindowCallback {

private:
    Options options_;
    std::unique_ptr<FPS> fps_;
    std::unique_ptr<DB> db_;
    std::unique_ptr<SceneManager> scene_manager_;
    std::unique_ptr<RenderSystem> render_system_;
    std::unique_ptr<ParticleSimulateSystem> particle_system_;
    std::unique_ptr<AppState> app_state_;
    std::unique_ptr<InputSystem> input_;
    std::unique_ptr<anim::AnimationSystem> anim_system_;
    std::unique_ptr<ScriptSystem> script_system_;

public:
    Game();
    Camera* Camera();
    void OnCreate(float w, float h, float ratio);
    void OnLoop();
    void OnDestroy();
    void OnPause();
    void OnResume();
    void OnFocusChanged(bool focused);
    void OnResize(float w, float h);
    void OnKeyEvent(int key, bool pressed);
    void OnPointEvent(int key, bool pressed, float x, float y);
    void SetGameSize(float w, float h);
    void Create(float w, float h, float ratio);
    void Destroy();
    void NotifyPause();
    void NotifyResume();
    void Init();
    void LoadTables();
    void Update();
    void DrawProfile();
    DB* GetDB()
    {
        return db_.get();
    }

    SceneManager* GetSceneManager()
    {
        return scene_manager_.get();
    }

    InputSystem* GetInputSystem()
    {
        return input_.get();
    }
};
} // namespace ant2d
