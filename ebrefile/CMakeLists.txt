# vi:set sw=2 et:
project(ebrefile C)

add_executable(ebrefile ebrefile.c)
target_link_libraries(ebrefile PRIVATE ebutils)
