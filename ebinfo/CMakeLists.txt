# vi:set sw=2 et:
project(ebinfo C)

add_executable(ebinfo ebinfo.c)
target_link_libraries(ebinfo PRIVATE ebutils)
