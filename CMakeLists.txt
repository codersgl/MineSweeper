set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 3.22)
project(MineSweeper LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Prefer an installed raylib; otherwise fetch the pinned version.
find_package(raylib 6.2 QUIET)
if(NOT raylib_FOUND)
  include(FetchContent)

  set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

  FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 6.2
  GIT_SHALLOW TRUE
  )
  FetchContent_MakeAvailable(raylib)
endif()


file(GLOB GAME_SOURCES CONFIGURE_DEPENDS
"${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")


add_executable(minesweeper ${GAME_SOURCES})

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(minesweeper PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_include_directories(minesweeper PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(minesweeper PRIVATE raylib)

if(UNIX AND NOT APPLE)
  target_link_libraries(minesweeper PRIVATE m)
endif()
