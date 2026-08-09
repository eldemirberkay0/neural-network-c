cmake_minimum_required(VERSION 3.18)
project(neural-network-c C)
include(FetchContent)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
message(STATUS "Fetching raylib from github...")
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib)

add_library(nn STATIC src/nn.c src/matrix.c)
target_include_directories(nn PUBLIC src)

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/examples/function-approximator/*.c")
add_executable(function_approximator ${SOURCES})
target_include_directories(function_approximator PRIVATE examples/function-approximator)
target_link_libraries(function_approximator nn raylib m)

# Download MNIST dataset for mnist-learner example
message(STATUS "Checking MNIST dataset...")
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/examples/mnist-learner/dataset/train-images.idx3-ubyte")
    message(STATUS "Downloading MNIST dataset... (11 MB)")
    file(DOWNLOAD 
        "https://github.com/eldemirberkay0/neural-network-c/releases/download/upload-dataset/mnist-dataset.zip"
        "${CMAKE_BINARY_DIR}/mnist-dataset.zip"
    )

    message(STATUS "Trying to extract dataset...")
    file(ARCHIVE_EXTRACT 
        INPUT "${CMAKE_BINARY_DIR}/mnist-dataset.zip"
        DESTINATION "${CMAKE_SOURCE_DIR}/examples/mnist-learner/dataset"
    )
    file(REMOVE "${CMAKE_BINARY_DIR}/mnist-dataset.zip")
else()
    message(STATUS "MNIST dataset is already downloaded.")
endif()

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/examples/mnist-learner/*.c")
file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/examples/mnist-learner/model")
add_executable(mnist_learner ${SOURCES})
target_include_directories(mnist_learner PRIVATE examples/mnist-learner)
target_link_libraries(mnist_learner raylib nn m)