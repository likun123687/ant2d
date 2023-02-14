#pragma once
#include <ant2d.h>
#include <lua_bind/lua_scene.h>
#include <lua_bind/lua_singleton.h>

/*
    std::string title;
    int width;
    int height;
    math::Vec4 clear;
    bool full_screen;
    bool no_title_bar;
    bool re_sizable;
*/
using namespace ant2d;

namespace {
template <typename T, typename U>
auto fetchIndex(T const& c, typename T::length_type idx) -> U const&
{
    if (idx < c.length()) {
        return c[idx];
    } else {
        throw std::logic_error("GLM index out of range");
    }
}

template <typename T, typename U>
auto fetchIndex(T& c, typename T::length_type idx) -> U&
{
    if (idx < c.length()) {
        return c[idx];
    } else {
        throw std::logic_error("GLM index out of range");
    }
}

template <typename T, typename U>
auto storeIndex(T& c, typename T::length_type idx, U const& v) -> void
{
    if (idx < c.length()) {
        c[idx] = v;
    } else {
        throw std::logic_error("GLM index out of range");
    }
}
}

void init_bind()
{
    sol::state& lua = SharedLua;
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::os, sol::lib::debug, sol::lib::io, sol::lib::table);

    auto ant2d_ns = lua["ant2d"].get_or_create<sol::table>();

    sol::usertype<ant2d::WindowOptions> options_type = ant2d_ns.new_usertype<ant2d::WindowOptions>("WindowOptions",
        sol::constructors<ant2d::WindowOptions()>());

    options_type["title"] = &ant2d::WindowOptions::title;
    options_type["width"] = &ant2d::WindowOptions::width;
    options_type["height"] = &ant2d::WindowOptions::height;
    // options_type["clear"] = &ant2d::WindowOptions::clear;
    options_type["full_screen"] = &ant2d::WindowOptions::full_screen;
    options_type["no_title_bar"] = &ant2d::WindowOptions::no_title_bar;
    options_type["re_sizable"] = &ant2d::WindowOptions::re_sizable;

    ant2d_ns.new_usertype<ant2d::Scene>("Scene",
        "OnEnter", &ant2d::Scene::OnEnter,
        "Update", &ant2d::Scene::Update,
        "OnExit", &ant2d::Scene::OnExit,
        "GetOnLoadCallback", &Scene::GetOnLoadCallback,
        "SetOnLoadCallback", &Scene::SetOnLoadCallback,
        "GetUnLoadCallback", &Scene::GetUnLoadCallback,
        "SetUnLoadCallback", &Scene::SetUnLoadCallback);

    ant2d_ns.new_usertype<LuaScene>("LuaScene",
        "OnEnter", &LuaScene::OnEnter,
        "Update", &LuaScene::Update,
        "OnExit", &LuaScene::OnExit,
        "GetOnLoadCallback", &LuaScene::GetOnLoadCallback,
        "SetOnLoadCallback", &LuaScene::SetOnLoadCallback,
        "GetUnLoadCallback", &LuaScene::GetUnLoadCallback,
        "SetUnLoadCallback", &LuaScene::SetUnLoadCallback,
        sol::meta_function::new_index, &LuaScene::Setter,
        sol::base_classes, sol::bases<ant2d::Scene>());

    ant2d_ns.set_function("Run", &ant2d::Run);

    ant2d_ns.new_usertype<Region>("Region",
        "x1", &Region::x1,
        "y1", &Region::y1,
        "x2", &Region::x2,
        "y2", &Region::y2,
        "rotated", &Region::rotated);

    ant2d_ns.new_usertype<ant2d::Size>("Size",
        "width", &ant2d::Size::width,
        "height", &ant2d::Size::height);

    ant2d_ns.new_usertype<ITexture2D>("ITexture2D",
        "GetTextureId", &ITexture2D::GetTextureId,
        "GetRegion", &ITexture2D::GetRegion,
        "GetSize", &ITexture2D::GetSize);

    ant2d_ns.new_usertype<BkTexture>("BkTexture",
        "GetTextureId", &ITexture2D::GetTextureId,
        "GetRegion", &ITexture2D::GetRegion,
        "GetSize", &ITexture2D::GetSize,
        sol::base_classes, sol::bases<ant2d::ITexture2D>());

    ant2d_ns.new_usertype<math::Vec2>("Vec2",
        sol::constructors<math::Vec2(), math::Vec2(float, float)>(),
        "Sub", &math::Vec2::Sub);

    ant2d_ns.new_usertype<Entity>("Entity",
        sol::constructors<Entity(), Entity(uint32_t)>(),
        "Gene", &Entity::Gene,
        "Index", &Entity::Index);

    ant2d_ns.new_usertype<TextureManager>("TextureManager",
        "Load", &TextureManager::Load,
        "Unload", &TextureManager::Unload,
        "LoadAtlas", &TextureManager::LoadAtlas,
        "LoadAtlasIndexed", &TextureManager::LoadAtlasIndexed,
        "Get", &TextureManager::Get);

    ant2d_ns.new_usertype<EntityManager>("EntityManager",
        "New", &EntityManager::New,
        "Alive", &EntityManager::Alive,
        "Destroy", &EntityManager::Destroy);

    ant2d_ns.new_usertype<SpriteComp>("SpriteComp",
        "SetSprite", &SpriteComp::SetSprite,
        "SetSize", &SpriteComp::SetSize,
        "GetSize", &SpriteComp::GetSize,
        "SetGravity", &SpriteComp::SetGravity,
        "GetGravity", &SpriteComp::GetGravity,
        "SetVisible", &SpriteComp::SetVisible,
        "GetVisible", &SpriteComp::GetVisible);

    ant2d_ns.new_usertype<SpriteTable>("SpriteTable",
        "NewComp", &SpriteTable::NewComp,
        "NewCompX", &SpriteTable::NewCompX);

    ant2d_ns.new_usertype<Transform>("Transform",
        "GetParentIdx", &Transform::GetParentIdx,
        "SetParentIdx", &Transform::SetParentIdx,
        "GetFirstChildIdx", &Transform::GetFirstChildIdx,
        "SetFirstChildIdx", &Transform::SetFirstChildIdx,
        "GetPreSiblingIdx", &Transform::GetPreSiblingIdx,
        "SetPreSiblingIdx", &Transform::SetPreSiblingIdx,
        "GetNxtSiblingIdx", &Transform::GetNxtSiblingIdx,
        "SetNxtSiblingIdx", &Transform::SetNxtSiblingIdx,
        "GetPosition", &Transform::GetPosition,
        "GetScale", &Transform::GetScale,
        "SetPosition", sol::resolve<void(math::Vec2)>(&Transform::SetPosition));

    ant2d_ns.new_usertype<TransformTable>("TransformTable",
        "NewComp", &TransformTable::NewComp,
        "TailDelete", &TransformTable::TailDelete,
        "Delete", &TransformTable::Delete);
}

void load_global()
{
    sol::state& lua = SharedLua;
    auto ant2d_ns = lua["ant2d"].get_or_create<sol::table>();
    ant2d_ns["SharedTextureManager"] = ant2d::SharedTextureManager;
    ant2d_ns["SharedEntityManager"] = ant2d::SharedEntityManager;
    ant2d_ns["SharedSpriteTable"] = ant2d::SharedSpriteTable;
    ant2d_ns["SharedTransformTable"] = ant2d::SharedTransformTable;
}