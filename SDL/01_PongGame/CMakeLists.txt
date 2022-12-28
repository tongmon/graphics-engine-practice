cmake_minimum_required(VERSION 3.16)

set(VCPKG_TARGET_TRIPLET "x64-windows-static" CACHE STRING "")
set(CMAKE_TOOLCHAIN_FILE C:/vcpkg/scripts/buildsystems/vcpkg.cmake)

add_custom_target(Graphviz ALL
    COMMAND ${CMAKE_COMMAND} "--graphviz=${CMAKE_BINARY_DIR}/Graph_Info/graph.dot" .
    COMMAND dot -Tpng ${CMAKE_BINARY_DIR}/Graph_Info/graph.dot -o ${CMAKE_BINARY_DIR}/../Graph_${CMAKE_BUILD_TYPE}.png
)

project(PONGGAME VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)
set(GAME_NAME Pong)

file(GLOB EXE_SOURCES CONFIGURE_DEPENDS ./*.cpp)

find_package(SDL2 CONFIG REQUIRED)

add_subdirectory(Game)

add_executable(${GAME_NAME} ${EXE_SOURCES})

target_link_libraries(${GAME_NAME} PUBLIC GameLib)