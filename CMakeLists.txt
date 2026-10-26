cmake_minimum_required(VERSION 3.15)
project(Calculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(calc 
    src/main.cpp
    src/expression_parser.cpp
    src/plugin_manager.cpp
)
if (NOT WIN32)
    target_link_libraries(calc PRIVATE dl)
endif()

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)
file(GLOB PLUGIN_SOURCES "${CMAKE_SOURCE_DIR}/plugins/*.cpp")

foreach(PLUGIN_SRC ${PLUGIN_SOURCES})
    get_filename_component(PLUGIN_NAME ${PLUGIN_SRC} NAME_WE)
    add_library(${PLUGIN_NAME} MODULE ${PLUGIN_SRC})
    target_include_directories(${PLUGIN_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    set_target_properties(${PLUGIN_NAME} PROPERTIES
        PREFIX ""
        OUTPUT_NAME "${PLUGIN_NAME}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
    )
    
    if (NOT WIN32)
        target_compile_options(${PLUGIN_NAME} PRIVATE -fvisibility=hidden)
    endif()
endforeach()
set_target_properties(calc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
