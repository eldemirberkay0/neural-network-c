cmake_minimum_required(VERSION 3.18)
project(neural-network-c C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(FetchContent)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
message(STATUS "Fetching raylib from github... (~100MB)")
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/6.0.zip
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
    message(STATUS "Downloading MNIST dataset... (~11 MB)")
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
    
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/examples/mnist-learner/dataset/train-images.idx3-ubyte")
        message(WARNING "Couldn't download MNIST dataset!")
    else()
        message(STATUS "MNIST dataset is downloaded successfully")
    endif()
else()
    message(STATUS "MNIST dataset is already downloaded")
endif()

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/examples/mnist-learner/*.c")
file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/examples/mnist-learner/model") # Ensure model directory is available
add_executable(mnist_learner ${SOURCES})
target_include_directories(mnist_learner PRIVATE examples/mnist-learner)
target_link_libraries(mnist_learner raylib nn m)

# Relative paths to access MNIST dataset and pre-trained model
target_compile_definitions(mnist_learner
    PRIVATE
    CMAKE_PATH_TRAIN_IMAGES="${CMAKE_SOURCE_DIR}/examples/mnist-learner/dataset/train-images.idx3-ubyte"
    CMAKE_PATH_TRAIN_LABELS="${CMAKE_SOURCE_DIR}/examples/mnist-learner/dataset/train-labels.idx1-ubyte"
    CMAKE_PATH_TEST_IMAGES="${CMAKE_SOURCE_DIR}/examples/mnist-learner/dataset/t10k-images.idx3-ubyte"
    CMAKE_PATH_TEST_LABELS="${CMAKE_SOURCE_DIR}/examples/mnist-learner/dataset/t10k-labels.idx1-ubyte"
    CMAKE_PATH_TRAINED_MODEL="${CMAKE_SOURCE_DIR}/examples/mnist-learner/model/model.nn"
)