# Use CMake commands to find the current Git branch or any other required info
# For example, using Git to get the current branch name
find_package(Git REQUIRED)
execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND ${GIT_EXECUTABLE} log -1 --format=%cd --date=format:"%m/%d/%y %H:%M:%S"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_TIME
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Generate a header file with preprocessor definitions
set(PRE_BUILD_HEADER "${CMAKE_BINARY_DIR}/src/_cmake/const.hpp")
file(WRITE ${PRE_BUILD_HEADER} "#pragma once\n")
file(APPEND ${PRE_BUILD_HEADER} "#define GIT_BRANCH \"${GIT_BRANCH}\"\n")
file(APPEND ${PRE_BUILD_HEADER} "#define GIT_COMMIT_HASH \"${GIT_COMMIT_HASH}\"\n")
file(APPEND ${PRE_BUILD_HEADER} "#define GIT_COMMIT_TIME ${GIT_COMMIT_TIME}\n")
