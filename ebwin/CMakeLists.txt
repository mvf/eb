# vi:set sw=2 et:
project(ebwin C)

add_library(ebwin STATIC dirent.c)
target_compile_definitions(ebwin PUBLIC _CRT_NONSTDC_NO_WARNINGS _CRT_SECURE_NO_WARNINGS)
target_include_directories(ebwin PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
