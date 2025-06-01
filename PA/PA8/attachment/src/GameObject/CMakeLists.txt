cmake_minimum_required(VERSION 3.16)

add_library(
  GameObject
  STATIC
  GameObject.cpp
  Zombie.cpp
  Plant.cpp
  Other.cpp
)

target_link_libraries(
  GameObject
  Framework
)
