
cmake_minimum_required(VERSION 3.23)
project(matrix-screen)


if(EMSCRIPTEN)
    set(
        CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -sUSE_SDL_TTF=2"
        )
else()
    target_link_libraries(
        ${PROJECT_NAME}
        INTERFACE
        SDL2_ttf
        )
endif()

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

target_include_directories(
    ${PROJECT_NAME}
    INTERFACE
    include
    )


target_link_libraries(
    ${PROJECT_NAME}
    INTERFACE
    sdlpp
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


