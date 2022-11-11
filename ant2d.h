#pragma once
#include <engi/entity.h>
#include <game/game.h>
#include <game/scene.h>
#include <gfx/sprite/sprite_table.h>
#include <gfx/transform/transform_table.h>
#include <gfx/mesh/mesh_table.h>
#include <hid/config.h>
#include <asset/texture_manager.h>
#include <effect/sim_gravity.h>
#include <effect/sim_fire.h>
#include <effect/sim_snow.h>

namespace ant2d {
extern Game* SharedGame;
extern SceneManager* SharedSceneManager;
extern EntityManager* SharedEntityManager;
extern SpriteTable* SharedSpriteTable;
extern MeshTable* SharedMeshTable;
extern ParticleSystemTable* SharedParticleSystemTable;
extern TransformTable* SharedTransformTable;
extern TextureManager* SharedTextureManager;
// extern InputSystem* SharedInputSystem;

void Run(WindowOptions options, Scene* sn);
}
