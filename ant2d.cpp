
#include <ant2d.h>
#include <hid/hid_manager.h>
#include <utils/debug.h>

namespace ant2d {

Game* SharedGame = nullptr;
SceneManager* SharedSceneManager = nullptr;
EntityManager* SharedEntityManager = nullptr;
SpriteTable* SharedSpriteTable = nullptr;
TransformTable* SharedTransformTable = nullptr;
TextureManager* SharedTextureManager = nullptr;
// InputSystem* SharedInputSystem = nullptr;

void Run(WindowOptions options, Scene* sn)
{
    Info("start to run, main scene--{}", static_cast<void*>(sn));
    auto g = new Game {};
    //g->Init();
    SharedGame = g;
    SharedTextureManager = new TextureManager{};
    SharedEntityManager = g->GetDB()->GetEntityManager();
    SharedSceneManager = g->GetSceneManager();
    SharedSceneManager->SetDefault(sn);
    for (auto& table : g->GetDB()->GetTableList()) {
        auto table_type = table->GetTableType();
        switch (table_type) {
        case TableType::kSprite:
            SharedSpriteTable = reinterpret_cast<SpriteTable*>(table.get());
            break;
        case TableType::kTransform:
            SharedTransformTable = reinterpret_cast<TransformTable*>(table.get());
            break;
        }
    }

    SharedHidManager.SetWindowCallback(g);
    SharedHidManager.SetOptions(options);
}

}
