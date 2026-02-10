utils.setLogLvl(2)
--add every files from ../src to path ( dont forget to add folders to path manually if you are unsing it)
package.path = package.path .. ";../src/?.lua"
init = false
function update()
    if not init then
        utils.windowed(600, 600)
        init = true
    end
    require("drawRectangle").my_function()
end

utils.initialize()
