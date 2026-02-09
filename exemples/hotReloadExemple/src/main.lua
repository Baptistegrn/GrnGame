utils.setLogLvl(0)
--add every files from ../src to path ( dont forget to add folders to path manually if you are unsing it)
package.path = package.path .. ";../src/?.lua"
function update()
    require("drawRectangle").my_function()
end

utils.initialize()
