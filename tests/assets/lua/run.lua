--print("aaaa")
local options = ant2d.WindowOptions.new()
options.title = "lua test"
options.width = 800
options.height = 600
print(options)

local on_load_callback = function()
    print("on load callback")
    ant2d.SharedTextureManager:Load("assets/face.png");
end

scene = ant2d.LuaScene.new()
scene:SetOnLoadCallback(on_load_callback)
function scene:OnEnterCb (g)
    print("lua on enter", g)
    local tex = ant2d.SharedTextureManager:Get("assets/face.png")
    local entity = ant2d.SharedEntityManager:New()
    local sprite = ant2d.SharedSpriteTable:NewCompX(entity, tex)

    sprite:SetSize(600, 600)

    local xf = ant2d.SharedTransformTable:NewComp(entity)
    xf:SetPosition(ant2d.Vec2.new(400, 400));
end

function scene:UpdateCb (dt)
    print("lua update", dt)
    print("memory", collectgarbage("count")) --打印当前的内存使用情况
end

function scene:OnExitCb ()
    print("lua OnExit")
end

local ret = ant2d.Run(options, scene)
print(ret)
return options