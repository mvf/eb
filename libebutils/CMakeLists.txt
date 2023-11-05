# vi:set sw=2 et:
project(ebutils C)

add_library(ebutils STATIC
  ebutils.c
  getumask.c
  makedir.c
  puts_eucjp.c
  samefile.c
  strcasecmp.c
  strlist.c
  yesno.c
)

if(WIN32)
  target_sources(ebutils PRIVATE getopt.c)
  set_property(SOURCE getopt.c APPEND PROPERTY COMPILE_DEFINITIONS REPLACE_GETOPT)
endif()

target_compile_definitions(ebutils INTERFACE
  VERSION="${eb_toplevel_VERSION}"
  MAILING_ADDRESS="m-kasahr@sra.co.jp"
  PUBLIC HAVE_CONFIG_H
)
target_link_libraries(ebutils PUBLIC eb)
target_include_directories(ebutils PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

set(localedir "${CMAKE_INSTALL_FULL_LOCALEDIR}")
configure_file(ebutils.h.in ebutils.h @ONLY)
