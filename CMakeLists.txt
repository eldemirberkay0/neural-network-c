cmake_minimum_required(VERSION 3.18)
project(neural-network-c C)
include(FetchContent)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
message("Fetching raylib from github...")
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib)

add_library(nn STATIC src/nn.c src/matrix.c)
target_include_directories(nn PUBLIC src)

file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/examples/function-approximator/*.c")
add_executable(function_approximator ${SOURCES})
target_include_directories(function_approximator PRIVATE examples/function-approximator)
target_link_libraries(function_approximator nn raylib m)

file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/examples/mnist-learner/*.c")
add_executable(mnist_learner ${SOURCES})
target_include_directories(mnist_learner PRIVATE examples/mnist-learner)
target_link_libraries(mnist_learner nn m)