cmake_minimum_required(VERSION 3.16)

project(PONGGAME VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)
set(GAME_NAME Pong)

file(GLOB EXE_SOURCES CONFIGURE_DEPENDS ./*.cpp)

include(FetchContent)

FetchContent_Declare(SDL2 GIT_REPOSITORY https://github.com/libsdl-org/SDL.git GIT_TAG release-2.26.1)
FetchContent_MakeAvailable(SDL2)

add_subdirectory(Game)

add_executable(${GAME_NAME} ${EXE_SOURCES})

target_link_libraries(${GAME_NAME} PUBLIC GameLib)