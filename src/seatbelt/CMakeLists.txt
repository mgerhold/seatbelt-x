include(${CMAKE_SOURCE_DIR}/cmake/system_link.cmake)
include(${CMAKE_SOURCE_DIR}/project_options.cmake)

add_library(seatbelt
        STATIC
        seatbelt.cpp
        include/seatbelt.hpp
        types.hpp
        lexer.hpp
        lexer.cpp
        tokens.hpp
        source_location.hpp
        utils.hpp
        utils.cpp
        lexer_error.hpp
        ostream_formatter.hpp
        parser.cpp
        parser.hpp
        statements/statement.hpp
        expressions/expression.hpp
        parser_error.hpp
        tokens.cpp
        executor.hpp
        interpreter/interpreter.cpp
        interpreter/interpreter.hpp
        statements/println.hpp
        expressions/u32_literal.hpp
        interpreter_error.hpp
        statements/print.hpp
)

target_link_libraries(seatbelt PRIVATE project_options)

target_include_directories(seatbelt
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_system_libraries(seatbelt PRIVATE utf8proc)
target_link_system_libraries(seatbelt PRIVATE ctre::ctre)
target_link_system_libraries(seatbelt PRIVATE magic_enum::magic_enum)
target_link_system_libraries(seatbelt PRIVATE Microsoft.GSL::GSL)
target_link_system_libraries(seatbelt PRIVATE tl::expected)
target_link_system_libraries(seatbelt PRIVATE tl::optional)
