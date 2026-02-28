-- the font dont accept all caracters (you can add somes of them manually in assets/font/)
window.setTitle("font exemple")
window.setUniversSize(320, 180)
image.loadFolder("assets/font")

local writing = ""
local save = {}
local cursor_timer = 0
local cursor_visible = true
local size_of_draw_text = 0

local frame_x = 40
local frame_y = 30
local frame_w = 240
local frame_h = 120
local padding = 6
local line_h = 12
local text_x = frame_x + padding
local text_y = frame_y + padding + 14

function update()
    local dt = var.delta()
    cursor_timer = cursor_timer + dt
    if cursor_timer >= 0.5 then
        cursor_visible = not cursor_visible
        cursor_timer = 0
    end

    writing = utils.getInputText() -- get the input of the user , have full time history
    if input.keyJustPressed("enter") then
        table.insert(save, writing)
        utils.deleteInputText() -- delete the entry of the user
        writing = ""
    end
    image.cls(30, 10, 100)
    -- draw the scene
    image.drawRectFilled(frame_x - 2, frame_y - 2, frame_w + 4, frame_h + 4, 0, 0, 0)
    image.drawRectFilled(frame_x, frame_y, frame_w, frame_h, 255, 255, 255)
    image.drawRectFilled(frame_x, frame_y, frame_w, 12, 30, 10, 100)
    image.drawText("assets/font", "text editor", frame_x + padding + 60, frame_y + 2, 1)
    image.drawRectFilled(frame_x + 1, frame_y + 13, frame_w - 2, frame_h - 14, 20, 20, 40)
    -- draw the history lines
    for i, line in ipairs(save) do
        image.drawText("assets/font", line, text_x, text_y + (i - 1) * line_h, 1)
    end
    -- draw the font and the cursor
    local current_y = text_y + #save * line_h
    size_of_draw_text = image.drawText("assets/font", writing, text_x, current_y, 1)
    image.drawRectFilled(text_x + size_of_draw_text - 1, current_y, 3, 8, 0, 0, 0)
    image.drawRectFilled(text_x + size_of_draw_text, current_y + 0.5, 1, 7, 255, 255, 255)
end

utils.setUpdateCallback(update)
