cmake_minimum_required(VERSION 3.0.0)
project(tuplet CXX)

add_library(tuplet INTERFACE)

option(tuplet_Install "Install CMake targets during install step." ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MSVC)
    target_compile_options(tuplet INTERFACE "/std:c++latest")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )
    target_Compile_options(tuplet INTERFACE "-std=c++2a")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
    target_compile_options(tuplet INTERFACE "-std=c++2a")
endif()

target_compile_definitions(tuplet INTERFACE)

target_include_directories(
    tuplet
    INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include>
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION include
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)

    file(GLOB all_tests "test/*.cpp")

    foreach(file_name ${all_tests})
        get_filename_component(test_name ${file_name} NAME_WE)
        add_executable("${test_name}" "${file_name}")
        target_link_libraries("${test_name}" PRIVATE tuplet)
        target_link_libraries("${test_name}" PRIVATE Threads::Threads)
        add_test(NAME "${test_name}" COMMAND "${test_name}")
    endforeach()
endif()


