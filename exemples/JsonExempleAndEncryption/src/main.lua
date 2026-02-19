-- You can compile to Lua bytecode and apply obfuscation
-- to your source code to further protect it from tampering.

-- Log configuration: 0 = Debug, 1 = Info, etc.
utils.setLogLvl(0)

local init = false
local r, g, b = 0, 0, 0
local FILE_NAME = "rect.json"

-- Setup security credentials
local function setupSecurity()
    -- Example key and IV format: key:00...01, iv:00...01
    -- Note: The key is reset to 0 every frame in the engine!
    json.setKey(15, 1)
    json.setIv(15, 1)
end

if json.exists(FILE_NAME) then
    -- If the file exists, we must set the key BEFORE loading to DECRYPT it.
    setupSecurity()
    json.load(FILE_NAME)
else
    -- If the file doesn't exist, we initialize a new one in memory.
    -- Note: Setting the key before a first 'load' on a new file won't affect it.
    json.load(FILE_NAME)

    -- Set default values (e.g., Red)
    json.writeVariable(FILE_NAME, "color.r", 255)
    json.writeVariable(FILE_NAME, "color.g", 0)
    json.writeVariable(FILE_NAME, "color.b", 0)

    -- Apply security credentials before saving to disk.
    setupSecurity()
    json.save(FILE_NAME)
end

-- Retrieve data from the JSON object and apply it to our variables.
r = json.readVariable(FILE_NAME, "color.r")
g = json.readVariable(FILE_NAME, "color.g")
b = json.readVariable(FILE_NAME, "color.b")

init = true

utils.setUpdateCallback(function()
    -- Render the rectangle using the loaded colors.
    image.drawRectFilled(100, 100, 20, 20, r, g, b, 255)
end)
