cmake_minimum_required(VERSION 3.14)

project(tuplet CXX)
add_library(tuplet INTERFACE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

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


include(${PROJECT_SOURCE_DIR}/helper.cmake)
if(PROJECT_IS_TOP_LEVEL)
    include(CTest)

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


