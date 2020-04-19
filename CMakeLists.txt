cmake_minimum_required(VERSION 3.10)
project(youtubedl_cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CURL_LIBRARY "-lcurl")
set(RE2_INCLUDE_DIR re2/re2/)

include(FindThreads)
find_package(CURL REQUIRED)
SET(JSON_INCLUDE "json/single_include/nlohmann/")

add_executable(youtubedl_cpp main.cpp)

target_link_libraries(youtubedl_cpp
        "libre2.a"
        ${CMAKE_THREAD_LIBS_INIT}
        ${CURL_LIBRARY})
target_include_directories(youtubedl_cpp PRIVATE ${JSON_INCLUDE} include/ ${RE2_INCLUDE_DIR} ${CURL_INCLUDE_DIR})