#include <game/game.h>
#include <gfx/gctx.h>
#include <gfx/sprite/sprite_table.h>
#include <gfx/transform/transform_table.h>
#include <gfx/batch_render.h>
#include <asset/shader_utils.h>
#include <gfx/sprite/sprite_render_feature.h>
#include <utils/debug.h>
#include <gfx/mesh/mesh_render.h>
#include <gfx/mesh/mesh_table.h>
#include <gfx/mesh/mesh_render_feature.h>
#include <effect/particle_render_feature.h>
#include <gfx/dbg/debug.h>
#include <gfx/text/text_table.h>
#include <gfx/text/text_render_feature.h>
#include <gui/ui_render_feature.h>
#include <gui/gui.h>

namespace ant2d {

void DB::AddTable(IBase* table)
{
    tables_.emplace_back(table);
}

TableList& DB::GetTableList()
{
    return tables_;
}

EntityManager* DB::GetEntityManager()
{
    return entity_manager_.get();
}

void DB::SetEntityManager(EntityManager* entity_manager)
{
    entity_manager_.reset(entity_manager);
}

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

Game::Game()
    : fps_(new FPS())
    , db_(new DB())
    , scene_manager_(new SceneManager())
    , render_system_(new RenderSystem())
    , particle_system_(new ParticleSimulateSystem())
    , app_state_(new AppState())
    , input_(new InputSystem {})
{
}

Camera* Game::Camera()
{
    return render_system_->GetMainCamera();
}

void Game::OnCreate(float w, float h, float ratio)
{
    Info("{}-{}-{}", w, h, ratio);
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

void Game::OnResize(float w, float h)
{
    // SetGameSize(w, h);
}

void Game::OnKeyEvent(int key, bool pressed)
{
    Info("on key event {}--{}", key, pressed);
    input_->SetKeyEvent(key, pressed);
}

void Game::OnPointEvent(int key, bool pressed, float x, float y)
{
    input_->SetPointerEvent(key, pressed, x, y);
}

void Game::SetGameSize(float w, float h)
{
    Info("game set size {}--{}", w, h);
    render_system_->GetMainCamera()->SetViewPort(w, h);
    // gui real screen size
    gui::SetScreenSize(w, h);
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
    render_system_->RegisterRender(new BatchRender(ShaderType::kBatchShader));
    render_system_->RegisterRender(new MeshRender(ShaderType::kMeshShader));

    // set feature
    auto srf = new SpriteRenderFeature {};
    srf->Register(render_system_.get());

    auto mrf = new MeshRenderFeature {};
    mrf->Register(render_system_.get());

    auto textf = new TextRenderFeature {};
    textf->Register(render_system_.get());

    auto ui = new UIRenderFeature {};
    ui->Register(render_system_.get());

    /// particle-simulation system
    particle_system_->RequireTable(&db_->GetTableList());
    // set feature
    auto prf = new ParticleRenderFeature();
    prf->Register(render_system_.get());

    /// setup scene manager
    scene_manager_->Setup(this);
}

void Game::Destroy()
{
    SharedDebug.Destroy();
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
    sprite_table->SetTableType(TableType::kSprite);
    db_->AddTable(sprite_table);

    auto transform_table = new TransformTable();
    transform_table->SetTableType(TableType::kTransform);
    db_->AddTable(transform_table);

    auto mesh_table = new MeshTable();
    mesh_table->SetTableType(TableType::KMesh);
    db_->AddTable(mesh_table);

    auto particle_system_table = new ParticleSystemTable();
    particle_system_table->SetTableType(TableType::kParticle);
    db_->AddTable(particle_system_table);

    auto text_table = new TextTable();
    text_table->SetTableType(TableType::kText);
    db_->AddTable(text_table);
}

void Game::Update()
{
    auto dt = fps_->Smooth();
    scene_manager_->Update(dt);
    particle_system_->Update(dt);

    // Render
    render_system_->Update(dt);

    DrawProfile();

    // flush drawCall
    int num = gfx::Flush();

    // drawCall = all-drawCall - camera-drawCall
    auto draw_call = num - render_system_->GetRenderList().size();
    SharedDebug.LogFPS(fps_->GetFPS(), draw_call);
}

void Game::DrawProfile()
{
    SharedDebug.AdvanceFrame();
}

}
