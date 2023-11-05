# vi:set sw=2 et:
cmake_minimum_required(VERSION 3.21)
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24")
  # Touch files extracted from downloaded archives
  cmake_policy(SET CMP0135 NEW)
endif()

# Visual Studio: Enable parallel builds
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
# Visual Studio: UTF-8 source and execution encoding, invalid UTF-8 in sources is an error
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8;/we4828>")

project(eb_toplevel)

include(cmake/config.h.cmake)

if(WIN32)
  add_subdirectory(ebwin)
endif()
add_subdirectory(eb)

option(EB_BUILD_UTILS "Build EB command-line utilities" ${PROJECT_IS_TOP_LEVEL})
option(EB_BUILD_SAMPLES "Build EB sample programs" OFF)

if(EB_BUILD_UTILS)
  add_subdirectory(libebutils EXCLUDE_FROM_ALL)
  add_subdirectory(ebappendix)
  add_subdirectory(ebfont)
  add_subdirectory(ebinfo)
  add_subdirectory(ebrefile)
  add_subdirectory(ebstopcode)
  add_subdirectory(ebzip)
endif()

if(EB_BUILD_SAMPLES)
  add_subdirectory(samples)
endif()
