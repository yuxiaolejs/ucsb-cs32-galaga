cmake_minimum_required(VERSION 3.14)
project(test)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PNG REQUIRED)
find_package(CURL REQUIRED )
find_package(CURLPP REQUIRED )
find_package(jsoncpp REQUIRED)
# find_package(PkgConfig REQUIRED)
# pkg_check_modules(JSONCPP jsoncpp)
# link_libraries(${JSONCPP_LIBRARIES})

file(GLOB ENGINE_LIB
     "src/lib/*.cpp"
     "src/3rdparty/*.cpp"
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
target_link_libraries(ezg curl)
target_link_libraries(ezg curlpp)
target_link_libraries(ezg jsoncpp)
# target_link_libraries(test )
# target_link_libraries(CanvasDownloader ${CURL_LIBRARIES})

target_include_directories(ezg PRIVATE /usr/include/jsoncpp)