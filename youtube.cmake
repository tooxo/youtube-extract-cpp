#                                           #
#   Include this in your CMakeLists File.   #
#                                           #
cmake_minimum_required(VERSION 3.10)

set(RE2_INCLUDE_DIR re2/re2/)

include(FindThreads)
find_package(CURL REQUIRED)
SET(JSON_INCLUDE "json/single_include/nlohmann/")

set(YOUTUBE_INCLUDE "${JSON_INCLUDE} include/ ${RE2_INCLUDE_DIR} ${CURL_INCLUDE_DIR}")