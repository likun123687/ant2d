options = ant2d.WindowOptions.new()
options.title = "lua test"
options.width = 800
options.height = 600
print(options)

scene = ant2d.LuaScene.new()
print(scene)

return ant2d.Run(options, scene)