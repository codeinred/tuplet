cmake_minimum_required(VERSION 3.14)

project(
    "tuplet"
    VERSION 2.1.1
    DESCRIPTION "A Lightweight Tuple Implementation for Modern C++"
    HOMEPAGE_URL "https://github.com/codeinred/tuplet/"
    LANGUAGES CXX)

add_library(tuplet INTERFACE)
add_library(
    tuplet::tuplet
    ALIAS
    tuplet)

# For ${CMAKE_INSTALL_<DIR>} variables that are standarized
include(GNUInstallDirs)

list(
    APPEND
    CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

target_compile_features(tuplet INTERFACE cxx_std_17)

target_include_directories(
    tuplet INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                     $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# Installation
# See: https://dominikberner.ch/cmake-interface-lib/

install(
    TARGETS tuplet
    EXPORT tuplet_Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "tupletConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/tupletConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/tupletConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/tuplet/cmake)

install(
    EXPORT tuplet_Targets
    FILE tupletTargets.cmake
    NAMESPACE tuplet::
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/tuplet/cmake)

install(FILES "${PROJECT_BINARY_DIR}/tupletConfig.cmake"
              "${PROJECT_BINARY_DIR}/tupletConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/tuplet/cmake)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/tuplet
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

include(helper)

option(
    BUILD_TESTING
    "Build testing for tuplet"
    ON)

if(PROJECT_IS_TOP_LEVEL AND BUILD_TESTING)
    # Enable exception handling on MSVC
    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
    endif()

    include(CTest)

    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)

    # Add all files in directory test as tests, and link against tuplet and
    # threads

    #if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    #    target_compile_options(
    #        test_kilo_tuple
    #        PRIVATE
    #        -fbracket-depth=1000)
    #endif()

    set(BENCHMARK_ENABLE_TESTING OFF)
    set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
    set(BENCHMARK_DOWNLOAD_DEPENDENCIES OFF)

    # Find the benchmark library, or download it in build/_deps
    find_or_fetch(
        benchmark
        https://github.com/google/benchmark.git
        main)
    find_or_fetch(
        fmt
        https://github.com/fmtlib/fmt.git
        master)
    find_or_fetch(
        Catch2
        https://github.com/catchorg/catch2.git
        v3.3.2
        3.3.2)
    FetchContent_MakeAvailable(${remote_dependencies})

    add_executable(
        bench
        bench/bench-heterogenous.cpp
        bench/bench-homogenous.cpp
        bench/bench-single-elem.cpp)

    file(GLOB test_files CONFIGURE_DEPENDS test/*.cpp)
    add_executable(test_tuplet ${test_files})
    target_link_libraries(
        test_tuplet
        tuplet::tuplet
        fmt::fmt
        Catch2::Catch2WithMain)
    target_link_libraries(
        bench
        tuplet::tuplet
        benchmark::benchmark_main)
    include(CTest)
    include(Catch)
    catch_discover_tests(test_tuplet)

    if(cxx_std_20 IN_LIST CMAKE_CXX_COMPILE_FEATURES)
        add_executable(test_tuplet_cpp_20 ${test_files})
        target_link_libraries(
            test_tuplet_cpp_20
            tuplet::tuplet
            fmt::fmt
            Catch2::Catch2WithMain)

        target_compile_features(test_tuplet_cpp_20 PRIVATE cxx_std_20)
        list(
            APPEND
            CMAKE_MODULE_PATH
            ${CMAKE_INSTALL_PREFIX}/lib/cmake/Catch2)
        list(
            APPEND
            CMAKE_MODULE_PATH
            ${catch2_SOURCE_DIR}/extras)

        catch_discover_tests(
            test_tuplet_cpp_20
            TEST_SUFFIX " (cpp 20)")
    endif()
endif()
