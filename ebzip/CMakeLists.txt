# vi:set sw=2 et:
project(ebzip C)

add_executable(ebzip
  copyfile.c
  ebzip.c
  ebzip1.c
  sebxa.c
  speedup.c
  unlinkfile.c
  unzipbook.c
  unzipfile.c
  zipbook.c
  zipfile.c
  zipinfobook.c
  zipinfofile.c
)

target_link_libraries(ebzip PRIVATE ebutils eb::zlib)
