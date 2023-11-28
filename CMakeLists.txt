cmake_minimum_required(VERSION 3.14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
project(test)
set(CMAKE_CXX_STANDARD 17)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PNG REQUIRED)
set(CMAKE_CXX_FLAGS "-g")
add_executable(ezg
    src/main.cpp
    src/controllers.cpp
    src/lib/ui.cpp
    src/lib/shape.cpp
    src/lib/texture.cpp
    src/lib/utils.cpp
    src/lib/game.cpp
    src/lib/layer.cpp
    src/lib/event.cpp
)
target_link_libraries(ezg glut OpenGL::GL OpenGL::GLU)
target_link_libraries(ezg png)

# target_link_libraries(test )
# target_link_libraries(CanvasDownloader ${CURL_LIBRARIES})