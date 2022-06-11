#include <game/game.h>
#include <gfx/gctx.h>
#include <gfx/sprite/sprite_table.h>
#include <gfx/transform/transform_table.h>

namespace ant2d {
void AppState::SetPaused(bool paused)
{
    old_.paused = now_.paused;
    now_.paused = paused;
}

void AppState::SetFocused(bool focused)
{
    old_.lost_focus = now_.lost_focus;
    now_.lost_focus = !focused;
}

AppState::StateChangeCallback AppState::GetPauseCallback()
{
    return pause_callback_;
}

AppState::StateChangeCallback AppState::GetFocusCallback()
{
    return focus_callback_;
}

Camera* Game::Camera()
{
    return render_system_->GetMainCamera();
}

void Game::OnCreate(float w, float h, float ratio)
{
    Create(w, h, ratio);
}

void Game::OnLoop()
{
    Update();
}

void Game::OnDestroy()
{
    Destroy();
}

void Game::OnPause()
{
    NotifyPause();
}

void Game::OnResume()
{
    NotifyResume();
}

void Game::OnFocusChanged(bool focused)
{
    app_state_->SetFocused(focused);
    auto fn = app_state_->GetFocusCallback();
    if (fn) {
        fn(focused);
    }
}

void Game::OnResize(int32_t w, int32_t h)
{
    SetGameSize(float(w), float(h));
}

void Game::SetGameSize(float w, float h)
{
    render_system_->GetMainCamera()->SetViewPort(w, h);
}

void Game::Create(float w, float h, float ratio)
{
    fps_->Init();
    gfx::Init(ratio);

    // render system
    render_system_.reset(new RenderSystem());
    SetGameSize(w, h);
    render_system_->GetMainCamera()->MoveTo(w / 2, h / 2);

    render_system_->RequireTable(db_->GetTableList());
    /*
    render_system_->RegisterRender(new BatchRender(ShaderType::BatchRender));

    // set feature
    auto srf = SpriteRenderFeature {};
    srf.Register(rs);

    /// setup scene manager
    sceneManager.Setup(g)
    */
}

void Game::Destroy()
{
    scene_manager_->Clear();
    render_system_->Destroy();
}

void Game::NotifyPause()
{
    app_state_->SetPaused(true);
    auto fn = app_state_->GetPauseCallback();
    if (fn) {
        fn(true);
    }
}

void Game::NotifyResume()
{
    app_state_->SetPaused(false);
    auto fn = app_state_->GetPauseCallback();
    if (fn) {
        fn(false);
    }
}

void Game::Init()
{
    LoadTables();
}

void Game::LoadTables()
{
    db_->SetEntityManager(new EntityManager());
    auto sprite_table = new SpriteTable();
    db_->AddTable(sprite_table);
    auto transform_table = new TransformTable();
    db_->AddTable(transform_table);
}

void Game::Update()
{
    auto dt = fps_->Smooth();
    scene_manager_->Update(dt);
    // Render
    render_system_->Update(dt);

    // flush drawCall
    int num = gfx::Flush();
}

}
