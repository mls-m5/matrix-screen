
cmake_minimum_required(VERSION 3.23)
project(matrix-screen)

add_subdirectory(
    lib/sdlpp
    )

add_library(
    ${PROJECT_NAME}
    INTERFACE
    )

list(APPEND
    CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/sdlpp/cmake/sdl2)
find_package(SDL2_ttf REQUIRED)

target_include_directories(
    ${PROJECT_NAME}
    INTERFACE
    include
    )

target_link_libraries(
    ${PROJECT_NAME}
    INTERFACE
    sdlpp
    SDL2::TTF
    )


target_compile_features(
    ${PROJECT_NAME}
    INTERFACE
    cxx_std_17
    )


# Demo application
add_executable(
    ${PROJECT_NAME}-demo
    demo/main.cpp
    )

target_link_libraries(
    ${PROJECT_NAME}-demo
    PRIVATE
    ${PROJECT_NAME}
    )


