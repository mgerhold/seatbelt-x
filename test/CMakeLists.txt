 CPMAddPackage(
         NAME GTEST
         GITHUB_REPOSITORY google/googletest
         VERSION 1.14.0
         OPTIONS
         "INSTALL_GTEST OFF"
 )

 add_executable(seatbelt_x_tests test_interpreter.cpp)
 target_link_libraries(seatbelt_x_tests PRIVATE seatbelt)
 target_link_system_libraries(seatbelt_x_tests PRIVATE gtest_main)

 include(GoogleTest)
 gtest_discover_tests(seatbelt_x_tests)
