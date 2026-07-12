cmake_minimum_required(VERSION 3.28)

include(FetchContent)

project(vexel VERSION 0.0.1 LANGUAGES C CXX)

add_executable(${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.8
)

FetchContent_MakeAvailable(SDL3)
FetchContent_GetProperties(
    SDL3
    SOURCE_DIR ${SDL3_SOURCE_DIR}
    BINARY_DIR ${SDL_BINARY_DIR}
    POPULATED ${SDL3_POPULATED}
)

if(NOT ${SDL3_POPULATED})
    FetchContent_Populate(SDL3)
    FetchContent_MakeAvailable(SDL3)

    FetchContent_GetProperties(
        SDL3
        SOURCE_DIR ${SDL3_SOURCE_DIR}
        BINARY_DIR ${SDL3_BINARY_DIR}
        POPULATED ${SDL3_POPULATED}
    )
endif()

target_sources(${PROJECT_NAME} PRIVATE "main.cpp")

add_subdirectory("deps")
# Generate OpenGL hooks by adding a custom target along with a custom command to run the glhooksgen program
add_custom_target(generate_debug_hooks DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/vexel/graphics/gl_hooks.h"
                                               "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/gl_hooks.cpp")

add_custom_command(OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/include/vexel/graphics/gl_hooks.h"
                          "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/gl_hooks.cpp"
                          
                   COMMAND $<TARGET_FILE:glhooksgen> -include vexel/graphics glCreateTextures glCreateBuffers glCreateSamplers glDeleteSamplers
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/deps/glhooksgen/output/gl_hooks.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/vexel/graphics/gl_hooks.h"
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/deps/glhooksgen/output/gl_hooks.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/gl_hooks.cpp"

                   DEPENDS glhooksgen)

add_dependencies(${PROJECT_NAME} generate_debug_hooks)
target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/gl_hooks.cpp)

add_subdirectory("src")

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
target_include_directories(${PROJECT_NAME} PRIVATE "${SDL3_SOURCE_DIR}/include")

target_link_libraries(${PROJECT_NAME} PRIVATE glad)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")