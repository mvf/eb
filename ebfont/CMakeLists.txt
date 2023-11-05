# vi:set sw=2 et:
project(ebfont C)

add_executable(ebfont ebfont.c)
target_link_libraries(ebfont PRIVATE ebutils)
