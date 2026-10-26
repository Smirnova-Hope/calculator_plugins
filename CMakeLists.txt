cmake_minimum_required(VERSION 3.15)
project(Calculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(calc src/main.cpp)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)
file(GLOB PLUGIN_SOURCES "${CMAKE_SOURCE_DIR}/plugins/*.cpp")

foreach(PLUGIN_SRC ${PLUGIN_SOURCES})
    get_filename_component(PLUGIN_NAME ${PLUGIN_SRC} NAME_WE)
    add_library(${PLUGIN_NAME} SHARED ${PLUGIN_SRC})
    set_target_properties(${PLUGIN_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
    )
endforeach()
