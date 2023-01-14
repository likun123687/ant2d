
#include <ant2d.h>
#include <hid/hid_manager.h>
#include <utils/debug.h>

namespace ant2d {

Game* SharedGame = nullptr;
SceneManager* SharedSceneManager = nullptr;
EntityManager* SharedEntityManager = nullptr;
SpriteTable* SharedSpriteTable = nullptr;
MeshTable* SharedMeshTable = nullptr;
ParticleSystemTable* SharedParticleSystemTable = nullptr;
TransformTable* SharedTransformTable = nullptr;
TextureManager* SharedTextureManager = nullptr;
FontManager* SharedFontManager = nullptr;
TextTable* SharedTextTable = nullptr;
AudioManager* SharedAudioManager = nullptr;
InputSystem* SharedInputSystem = nullptr;
frame::FlipbookTable* SharedFlipbookTable = nullptr;

void Run(WindowOptions options, Scene* sn)
{
    auto g = new Game {};
    g->Init();
    SharedGame = g;
    SharedTextureManager = new TextureManager {};
    SharedFontManager = new FontManager {};
    SharedAudioManager = new AudioManager {};
    SharedEntityManager = g->GetDB()->GetEntityManager();
    SharedSceneManager = g->GetSceneManager();
    SharedSceneManager->SetDefault(sn);
    SharedInputSystem = g->GetInputSystem();
    for (auto& table : g->GetDB()->GetTableList()) {
        auto table_type = table->GetTableType();
        switch (table_type) {
        case TableType::kSprite:
            SharedSpriteTable = reinterpret_cast<SpriteTable*>(table.get());
            break;
        case TableType::kTransform:
            SharedTransformTable = reinterpret_cast<TransformTable*>(table.get());
            break;
        case TableType::KMesh:
            SharedMeshTable = reinterpret_cast<MeshTable*>(table.get());
            break;
        case TableType::kParticle:
            SharedParticleSystemTable = reinterpret_cast<ParticleSystemTable*>(table.get());
            break;
        case TableType::kText:
            SharedTextTable = reinterpret_cast<TextTable*>(table.get());
            break;
        case TableType::kFlipbook:
            SharedFlipbookTable = reinterpret_cast<frame::FlipbookTable*>(table.get());
            break;
        }
    }

    SharedHidManager.SetWindowCallback(g);
    SharedHidManager.SetOptions(options);
}

}
