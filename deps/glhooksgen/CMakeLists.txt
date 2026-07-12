cmake_minimum_required(VERSION 3.28)

project(glhooksgen)
add_executable(glhooksgen "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_sources(${PROJECT_NAME} PRIVATE "${source_files}")

target_compile_definitions(${PROJECT_NAME} PRIVATE "-DCMAKE_CURRENT_SOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\"" "-DENGINE_NAME=\"vexel\"") # Yes the engine name is hardcoded
