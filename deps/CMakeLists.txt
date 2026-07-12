cmake_minimum_required(VERSION 3.28)

file(GLOB files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*")

foreach(file IN LISTS files)
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${file}")
        add_subdirectory("${file}")
    endif()
endforeach()