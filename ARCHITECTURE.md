# Files
Each .h / .c duo should have **one clear job**.

## /app
What launches your game

## /tests
Temp tests used throughout the dev process

## /grngame

### /assets
Loading/unloading multiple types of assets

### /audio
Playing sound

### /physics
2D platformer physics

### /input
Manages inputs received from the window

### /platform
Wrappers over OS specific functionality (eg. filepaths)

### /math
Functions and utilites related to math that are not found in cglm

### /bindings
Binding everything to Lua/Dascript

### /renderer
Rendering related functions and structs

# Naming conventions
## Casing
- **Types:** PascalCase
- **Fonctions:** PascalCase
- **Macros:** UPPER_SNAKE_CASE
- **Autre:** snake_case
## Naming
Functions that operate a struct or are part of a "module" are "noun first", examples:
- `WindowCreate`
- `PhysicsTick`
- `AudioLoad`

# Misc
- Ideally a function should not be longer than 15 lines
- If something can be extracted to a utility function, do it, and if the contents of the functions could be reused somewhere, place it somewhere appropriate and make it public
- No logical operation should be split accross multiple files (or even functions)
- You can break rules if it helps readability