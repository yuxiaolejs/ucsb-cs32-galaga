# Minimum Viable Product!

Made OpenGL work for us!

## Branch information
Related branches
- w2-assets-b01
- w2-endgame-b01
- w2-endscrn-b01
- w2-fix-b01
- w2-projectile-b01
- w2-projectile-b02
- w2-text-b01

## File structure
```
master
├── CMakeCache.txt    ----- CMakefile
├── docs    --------------- Documentation
│   ├── ckpt1.md
│   ├── ckpt2.md
│   ├── index.html
│   └── README.md
├── Makefile
├── README.md
├── res    ---------------- Resource files
│   └── **    ------------- Images for texture
└── src    ---------------- Source file
    ├── 3rdparty   -------- Third party libs
    │   ├── request.cpp
    │   └── request.hpp
    ├── controllers.cpp  -- Main game logic
    ├── controllers.hpp  -- Game controller defn
    ├── lib    ------------ A simple "game engine"
    │   ├── event.cpp
    │   ├── event.hpp
    │   ├── game.cpp
    │   ├── game.hpp
    │   ├── layer.cpp
    │   ├── layer.hpp
    │   ├── performance.cpp
    │   ├── performance.hpp
    │   ├── shape.cpp
    │   ├── shape.hpp
    │   ├── texture.cpp
    │   ├── texture.hpp
    │   ├── uiconf.cpp
    │   ├── uiconf.hpp
    │   ├── ui.cpp
    │   ├── ui.hpp
    │   ├── utils.cpp
    │   └── utils.hpp
    ├── main.cpp    -------- Entry point
    ├── projectile.cpp   --- Physical logic
    ├── projectile.hpp   --- Projectile defn
    ├── text.cpp    -------- Dynamic font
    ├── text.hpp    -------- Font defn
    └── vec.hpp    --------- Vector class

```

## Testing information
### CSIL tests
Passed when running in RDP
### Automated tests
*Only testing if binary can be made*

## Features
- End screen
- Leader board
- Two types of projectile
- Player hp calculation
- Performance manager