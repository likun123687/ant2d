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

void init_bind()
{
    sol::state& lua = SharedLua;
    lua.open_libraries(sol::lib::base);
    using namespace ant2d;

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

    ant2d_ns.new_usertype<ant2d::Scene>("Scene", "OnEnter", &ant2d::Scene::OnEnter, "Update", &ant2d::Scene::Update, "OnExit", &ant2d::Scene::OnExit);
    ant2d_ns.new_usertype<LuaScene>("LuaScene", "OnEnter", &LuaScene::OnEnter, "Update", &LuaScene::Update, "OnExit", &LuaScene::OnExit,
        sol::base_classes, sol::bases<ant2d::Scene>());

    ant2d_ns.set_function("Run", &ant2d::Run);

    ant2d_ns.new_usertype<Region>("Region",
        "x1", &Region::x1,
        "y1", &Region::y1,
        "x2", &Region::x2,
        "y2", &Region::y2,
        "rotated", &Region::rotated);

    ant2d_ns.new_usertype<Size>("Size",
        "width", &Size::width,
        "height", &Size::height);

    ant2d_ns.new_usertype<ITexture2D>("ITexture2D",
        "GetTextureId", &ITexture2D::GetTextureId,
        "GetRegion", &ITexture2D::GetRegion,
        "GetSize", &ITexture2D::GetSize);

    ant2d_ns.new_usertype<BkTexture>("BkTexture",
        "GetTextureId", &ITexture2D::GetTextureId,
        "GetRegion", &ITexture2D::GetRegion,
        "GetSize", &ITexture2D::GetSize,
        sol::base_classes, sol::bases<ant2d::ITexture2D>());

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

    ant2d_ns.new_usertype<SpriteTable>("SpriteTable",
        "NewComp", &SpriteTable::NewComp,
        "NewCompX", &SpriteTable::NewCompX);

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
}