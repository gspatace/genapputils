cmake_minimum_required (VERSION 3.8)

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/include/envvars")
include_directories("${PROJECT_SOURCE_DIR}/include/smartparams")

add_definitions(-DGENAPPUTILS_EXPORTS)

add_library(genapputils SHARED EnvVarItem.cpp EnvVarRegistrar.cpp EnvVarCache.cpp ParamItem.cpp )