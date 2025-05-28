cmake_minimum_required (VERSION 3.10)
project (SOIL)

file (GLOB SOIL_SOURCES src/*.c)
file (GLOB SOIL_HEADERS include/*.h)

add_library (
  SOIL 
  STATIC
  ${SOIL_HEADERS}
  ${SOIL_SOURCES} 
)

target_link_libraries(
  SOIL
)

target_include_directories (
  SOIL 
  PUBLIC
  include/
)
