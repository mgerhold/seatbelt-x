cmake_minimum_required(VERSION 3.26)
project(seatbelt_x
        VERSION 0.0.1
        DESCRIPTION "the definitive Backseat compiler"
        HOMEPAGE_URL "https://github.com/mgerhold/backseat-x"
        LANGUAGES C CXX  # C is required by some of the dependencies
)

if ("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
    message(FATAL_ERROR "In-source builds are not allowed")
endif ()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# don't allow the usage of compiler-specific extensions to improve portability
set(CMAKE_CXX_EXTENSIONS OFF)

include(project_options.cmake)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# activate LTO (link-time optimizations) to increase the chance of catching ODR violations
include(cmake/lto.cmake)
try_enable_link_time_optimizations()

include(${CMAKE_SOURCE_DIR}/dependencies.cmake)
setup_dependencies()

add_subdirectory(src bin)

if (seatbelt_x_build_tests)
    enable_testing()
    add_subdirectory(test)
endif ()
