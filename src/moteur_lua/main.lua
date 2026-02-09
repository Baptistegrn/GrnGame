utils.setLogLvl(0)
--add every files from ../src to path ( dont forget to add folders to path manually if you are unsing it)
package.path = package.path .. ";../src/?.lua"

function update()
    --update your logic here
end

utils.initialize()
