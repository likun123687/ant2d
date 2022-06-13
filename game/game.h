#pragma once

#include <engi/entity.h>
#include <game/fps.h>
#include <engi/base_table.h>
#include <game/scene.h>
#include <gfx/render_system.h>

namespace ant2d {
const int32_t kMaxScriptSize = 1024;
const int32_t kMaxSpriteSize = 64 << 10;
const int32_t kMaxTransformSize = 64 << 10;
const int32_t kMaxTextSize = 64 << 10;
const int32_t kMaxMeshSize = 64 << 10;
const int32_t kMaxParticleSize = 1024;

struct Options {
    int32_t width;
    int32_t height;
};

class DB {
private:
    std::unique_ptr<EntityManager> entity_manager_;
    TableList tables_;
public:
    void AddTable(IBase *table);
    TableList& GetTableList();
    EntityManager* GetEntityManager();
    void SetEntityManager(EntityManager *entity_manager);
};

class AppState {
using StateChangeCallback = std::function<void(bool)>;

private:
    StateChangeCallback pause_callback_;
    StateChangeCallback focus_callback_;
    struct State {
        bool paused;
        bool lost_focus;
    };

    State old_;
    State now_;

public:
    void SetPaused(bool paused);
    void SetFocused(bool focused);
    StateChangeCallback GetPauseCallback();
    StateChangeCallback GetFocusCallback();
};

class Game {

private:
    Options options_;
    std::unique_ptr<FPS> fps_;
    std::unique_ptr<DB> db_;
    std::unique_ptr<SceneManager> scene_manager_;
    std::unique_ptr<RenderSystem> render_system_;
    std::unique_ptr<AppState> app_state_;

public:
    Camera* Camera();
    void OnCreate(float w, float h, float ratio);
    void OnLoop();
    void OnDestroy();
    void OnPause();
    void OnResume();
    void OnFocusChanged(bool focused);
    void OnResize(int32_t w, int32_t h);
    void SetGameSize(float w, float h);
    void Create(float w, float h, float ratio);
    void Destroy();
    void NotifyPause();
    void NotifyResume();
    void Init();
    void LoadTables();
    void Update();
};
} // namespace ant2d
