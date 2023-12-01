# Hello OpneGL!

Made OpenGL work for us!

## Branch information
Related branches
- w1-fonts-b01
- w1-galaga-b03
- w1-glengine-b01
- w1-glengine-b02
- w1-threding-b01

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
│   ├── _alpha.png
│   ├── alpha.png
│   ├── LB_TEXT.png
│   ├── logo.png
│   ├── PRESS_C.png
│   └── ship1.png
└── src    ---------------- Source file
    ├── controllers.cpp
    ├── controllers.hpp
    ├── lib    ------------ A simple "game engine"
    │   ├── event.cpp
    │   ├── event.hpp
    │   ├── game.cpp
    │   ├── game.hpp
    │   ├── layer.cpp
    │   ├── layer.hpp
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
    └── main.cpp    -------- Entry point
```

## Testing information
### CSIL tests
Passed when running in RDP
### Automated tests
*Only testing if binary can be made*