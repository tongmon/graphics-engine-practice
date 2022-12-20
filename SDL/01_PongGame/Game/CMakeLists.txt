set(GAMELIB_NAME GameLib)

file(GLOB LIB_SOURCES CONFIGURE_DEPENDS ./*.cpp)

add_library(${GAMELIB_NAME} STATIC ${LIB_SOURCES})

target_link_libraries(${GAMELIB_NAME} PUBLIC SDL2::SDL2-static)

target_include_directories(${GAMELIB_NAME} PUBLIC "./")