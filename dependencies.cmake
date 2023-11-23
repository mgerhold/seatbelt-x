include("${CMAKE_SOURCE_DIR}/cmake/CPM.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/system_link.cmake")

function(setup_dependencies)
    CPMAddPackage(
            NAME UTF8_PROC
            GITHUB_REPOSITORY JuliaStrings/utf8proc
            VERSION 2.9.0
            OPTIONS
            "UTF8PROC_INSTALL OFF"
    )
    CPMAddPackage(
            NAME COMPILE_TIME_REGULAR_EXPRESSIONS
            GITHUB_REPOSITORY hanickadot/compile-time-regular-expressions
            VERSION 3.8.1
    )
    CPMAddPackage(
            NAME NLOHMANN_JSON
            GITHUB_REPOSITORY nlohmann/json
            VERSION 3.10.5
            OPTIONS
            "JSON_BuildTests OFF"
            "JSON_CI OFF"
    )
    CPMAddPackage(
            NAME TL_OPTIONAL
            GITHUB_REPOSITORY TartanLlama/optional
            VERSION 1.1.0
            OPTIONS
            "OPTIONAL_BUILD_TESTS OFF"
            "OPTIONAL_BUILD_PACKAGE OFF"
            "DPKG_BUILDPACKAGE_FOUND OFF"
            "OPTIONAL_BUILD_PACKAGE OFF"
            "RPMBUILD_FOUND OFF"
            "OPTIONAL_BUILD_PACKAGE OFF"
            "CMAKE_HOST_WIN32 OFF"
    )
    CPMAddPackage(
            NAME TL_EXPECTED
            GITHUB_REPOSITORY TartanLlama/expected
            VERSION 1.1.0
            OPTIONS
            "EXPECTED_BUILD_TESTS OFF"
            "EXPECTED_BUILD_PACKAGE_DEB OFF"
    )
    CPMAddPackage(
            NAME GSL
            GITHUB_REPOSITORY microsoft/GSL
            VERSION 4.0.0
            OPTIONS
            "GSL_TEST OFF"
    )
    CPMAddPackage(
            NAME MAGIC_ENUM
            GITHUB_REPOSITORY Neargye/magic_enum
            VERSION 0.9.3
            OPTIONS
            "MAGIC_ENUM_OPT_BUILD_EXAMPLES OFF"
            "MAGIC_ENUM_OPT_BUILD_TESTS OFF"
    )
endfunction()
