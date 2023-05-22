cmake_minimum_required(VERSION 3.24)
project(JSONParser)

set(CMAKE_CXX_STANDARD 17)

add_executable(JSONParser src/main.cpp src/JsonValue.cpp src/JsonParser.cpp)