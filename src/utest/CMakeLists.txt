cmake_minimum_required (VERSION 3.8)

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/include/envvars")
include_directories("${PROJECT_SOURCE_DIR}/include/smartparams")

add_executable(app main.cpp)

target_link_libraries(app genapputils)