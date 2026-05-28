import "std/wren/renderer/color" for Color

class Palette {
    construct new() {
        _paletteElements = []
    }

    add(color) {
        _paletteElements.add(color)
    }

    remove(index) {
        _paletteElements.removeAt(index)
    }

    clear() {
        _paletteElements.clear()
    }

    count {
        _paletteElements.count
    }

    get(index) {
        _paletteElements[index]
    }

    set(index, color) {
        _paletteElements[index] = color
    }

    contains(color) {
        _paletteElements.contains(color)
    }

    each(fn) {
        for (color in _paletteElements) {
            fn.call(color)
        }
    }

    colors {
        _paletteElements
    }

}