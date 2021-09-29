cmake_minimum_required(VERSION 3.14)

project(tuplet CXX)
add_library(tuplet INTERFACE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MSVC)
    target_compile_options(tuplet INTERFACE "/std:c++latest" "/EHsc")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )
    target_Compile_options(tuplet INTERFACE "-std=c++2a")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
    target_compile_options(tuplet INTERFACE "-std=c++2a")
endif()


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


if(PROJECT_IS_TOP_LEVEL)
    include(CTest)
    include(${PROJECT_SOURCE_DIR}/helper.cmake)

    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)

    # Add all files in directory test as tests, and link against tuplet and
    # threads
    add_test_dir(test tuplet Threads::Threads)

    set(BENCHMARK_ENABLE_TESTING OFF)
    set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
    set(BENCHMARK_DOWNLOAD_DEPENDENCIES OFF)

    # Find the benchmark library, or download it in build/_deps
    find_or_fetch(
        benchmark
        https://github.com/google/benchmark.git
        main)
    FetchContent_MakeAvailable(${remote_dependencies})

    add_source_dir(
        bench
        tuplet
        benchmark)
endif()


