cmake_minimum_required(VERSION 3.0.0)
project(Renderer VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB_RECURSE SRCS "*.cc")

add_executable(Renderer ${SRCS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
