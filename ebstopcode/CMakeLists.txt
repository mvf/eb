# vi:set sw=2 et:
project(ebstopcode C)

add_executable(ebstopcode ebstopcode.c)
target_link_libraries(ebstopcode PRIVATE ebutils)
