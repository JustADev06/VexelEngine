cmake_minimum_required(VERSION 3.28)

file(GLOB_RECURSE source_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

target_sources(${PROJECT_NAME} PRIVATE ${source_files})