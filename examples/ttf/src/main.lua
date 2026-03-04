window.setTitle("font exemple")
window.setUniversSize(320, 180)

function update()
    image.drawTextTtf("comic.ttf", "hello ttf", 200, 80, 80, 255, 0, 0, 255);
    image.drawTextTtf("indiafontmodiarjunjavadev.ttf", "हैलो वर्ल्ड", 200, 80, 600, 255, 255, 255, 255);
end

utils.setUpdateCallback(update)
