cmake_minimum_required(VERSION 3.28)

project(openmp-nodejs CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD 1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
    # Set _ITERATOR_DEBUG_LEVEL to 0 (required by CEF)
    add_compile_definitions($<$<CONFIG:Debug>:_ITERATOR_DEBUG_LEVEL=0>)
endif()

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_INSTALL_RPATH "$ORIGIN")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_subdirectory(dependencies)
add_subdirectory(openmp-nodejs)
