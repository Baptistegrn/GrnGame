--log debug
utils.setLogLvl(0)

--path
package.path = package.path .. ";./src/?.lua"

function update()
    --resive
    if input.keyJustPressed("f3") then
        utils.windowed(500, 500)
    end
    --add to path if you add requires from another folder
    require("test").draw()
end

-- auto arguments
utils.initialize()
