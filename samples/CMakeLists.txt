# vi:set sw=2 et:
project(samples C)

foreach(target appendix booklist disctype font initexit subbook text word)
  add_executable(${target} ${target}.c)
  target_link_libraries(${target} PRIVATE eb::eb)
endforeach()
