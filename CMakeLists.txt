cmake_minimum_required(VERSION 3.14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
project(test)
set(CMAKE_CXX_STANDARD 17)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PNG REQUIRED)
set(CMAKE_CXX_FLAGS "-g")
file(GLOB ENGINE_LIB
     "src/lib/*.cpp"
)
file(GLOB GAME_BIZ
     "src/*.cpp"
)
add_executable(ezg
    ${GAME_BIZ}
    ${ENGINE_LIB}
)
target_link_libraries(ezg glut OpenGL::GL OpenGL::GLU)
target_link_libraries(ezg png)

# target_link_libraries(test )
# target_link_libraries(CanvasDownloader ${CURL_LIBRARIES})