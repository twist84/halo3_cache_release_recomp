# halo3_cache_release - ReXGlue Recompiled Project
#
# This file is yours to edit. 'rexglue migrate' will NOT overwrite it.
# SDK boilerplate lives in generated/rexglue.cmake.

cmake_minimum_required(VERSION 3.25)
project(halo3_cache_release LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(generated/rexglue.cmake)

# Sources
set(HALO3CACHERELEASE_SOURCES
    source/main.cpp
)

if(WIN32)
    add_executable(halo3_cache_release WIN32 ${HALO3CACHERELEASE_SOURCES})
else()
    add_executable(halo3_cache_release ${HALO3CACHERELEASE_SOURCES})
endif()

file(GLOB_RECURSE HALO3_GAME_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/halo3/source/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/halo3/source/*.c"
)
target_sources(halo3_cache_release PRIVATE
    ${HALO3_GAME_SOURCES}
)
target_include_directories(halo3_cache_release PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/halo3/source
)

rexglue_setup_target(halo3_cache_release)

