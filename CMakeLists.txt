cmake_minimum_required(VERSION 3.24)
project(JsonParser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CSS_STANDARD_REQUIRED True)

add_subdirectory(src)
include_directories(include)
add_compile_options(-Wall -Wextra)