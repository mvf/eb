# vi:set sw=2 et:
project(eb LANGUAGES C VERSION 4.4.3)

# Keep in sync with LIBEB_VERSION_INFO in configure*
set(EB_SONAME 16)

add_library(eb
  appendix.c
  appsub.c
  bcd.c
  binary.c
  bitmap.c
  book.c
  booklist.c
  copyright.c
  cross.c
  eb.c
  eb.def
  endword.c
  error.c
  exactword.c
  filename.c
  font.c
  hook.c
  jacode.c
  keyword.c
  lock.c
  log.c
  match.c
  menu.c
  multi.c
  narwalt.c
  narwfont.c
  readtext.c
  search.c
  setword.c
  stamp-widealt-h
  stamp-widefont-h
  stopcode.c
  strcasecmp.c
  subbook.c
  text.c
  widealt.c
  widefont.c
  word.c
  zio.c
)

if(WIN32)
  target_link_libraries(eb PUBLIC ebwin)
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/eb.rc.in" "${CMAKE_CURRENT_BINARY_DIR}/eb.rc" @ONLY)
  target_sources(eb PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/eb.rc")
else()
  option(EB_ENABLE_EBNET "Enable EBNET support" OFF)
endif()

if(EB_ENABLE_EBNET)
  target_sources(eb PRIVATE ebnet.c linebuf.c multiplex.c urlparts.c)
endif()

target_compile_definitions(eb PRIVATE EB_BUILD_LIBRARY HAVE_CONFIG_H)

target_include_directories(eb
  PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/.."
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

include(GNUInstallDirs)
set(localedir "${CMAKE_INSTALL_FULL_LOCALEDIR}")
configure_file(cmake/sysdefs.h.in sysdefs.h @ONLY)
configure_file(build-post.h.in build-post.h @ONLY)

add_library(eb_zlib INTERFACE)
find_package(ZLIB)
if(ZLIB_FOUND)
  target_link_libraries(eb_zlib INTERFACE ZLIB::ZLIB)
else()
  set(EB_ZLIB_VERSION "1.3" CACHE STRING "ZLIB version when building from source")
  message(STATUS "ZLIB not found, building version ${EB_ZLIB_VERSION} from source")
  include(FetchContent)
  FetchContent_Declare(zlib URL https://github.com/madler/zlib/releases/download/v${EB_ZLIB_VERSION}/zlib-${EB_ZLIB_VERSION}.tar.gz)
  # Can't use FetchContent_MakeAvailable on CMake < 3.28 due to:
  # https://gitlab.kitware.com/cmake/cmake/-/issues/20167
  if(NOT zlib_POPULATED)
    FetchContent_Populate(zlib)
    add_subdirectory("${zlib_SOURCE_DIR}" "${zlib_BINARY_DIR}" EXCLUDE_FROM_ALL)
  endif()
  get_target_property(eb_type eb TYPE)
  # Link static zlib when static, shared zlib when shared. Static zlib + shared
  # EB is a bad idea (except on Windows) due to PIC-ness and also because EB
  # doesn't have a proper library interface and will export zlib symbols, with
  # unpredictable effects.
  if("${eb_type}" STREQUAL "SHARED_LIBRARY")
    target_link_libraries(eb_zlib INTERFACE zlib)
  else()
    target_link_libraries(eb_zlib INTERFACE zlibstatic)
  endif()
  # https://github.com/madler/zlib/issues/133
  target_include_directories(eb_zlib INTERFACE "${zlib_SOURCE_DIR}" "${zlib_BINARY_DIR}")
endif()
add_library(eb::zlib ALIAS eb_zlib)
target_link_libraries(eb PRIVATE eb::zlib)

set_target_properties(eb PROPERTIES SOVERSION ${EB_SONAME})
add_library(eb::eb ALIAS eb)
