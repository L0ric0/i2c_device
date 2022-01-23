cmake_minimum_required(VERSION 3.10)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
set(CMAKE_TOOLCHAIN_FILE "vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(i2c_device VERSION 0.0.1)

include(PreventInSourceBuilds)

include(StandardProjectSettings)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)

add_library(project_warnings INTERFACE)
include(CompilerWarnings)
set_project_warnings(project_options)

include(StaticAnalyzers)

include(Doxygen)
enable_doxygen()

option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
#option(ENABLE_TESTING "Enable Test Builds" OFF)
#option(ENABLE_COVERAGE "Generate coverage reports." OFF)
#option(ENABLE_FUZZING "Enable Fuzzing Builds" OFF)

#if(ENABLE_TESTING)
#    enable_testing()
#    message("Building Tests.")
#    if(ENABLE_COVERAGE)
#        include(CodeCoverage)
#    endif()
#    add_subdirectory(tests)
#endif()

add_subdirectory(i2c_device)
