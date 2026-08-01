cmake_minimum_required(VERSION 3.10)
project(neural-network-c C)

add_library(nn STATIC src/nn.c src/matrix.c)
target_include_directories(nn PUBLIC src)