#include "include/seatbelt.hpp"
#include "compiler/compiler.hpp"
#include "executor.hpp"
#include "interpreter/interpreter.hpp"
#include "interpreter_error.hpp"
#include "lexer.hpp"
#include "lexer_error.hpp"
#include "parser.hpp"
#include "parser_error.hpp"
#include <fstream>
#include <iostream>
#include <magic_enum.hpp>
#include <reproc++/run.hpp>

static void execute(std::vector<statements::Statement> const& statements, Executor& executor) {
    for (auto const& statement : statements) {
        if (auto const& println = statement->as_println()) {
            executor.println(println.value());
        } else if (auto const& print = statement->as_print()) {
            executor.print(print.value());
        }
    }
}

static void try_tokenize_parse_execute(std::filesystem::path const& path, Executor& executor) {
    using namespace std::string_view_literals;
    auto const file = utils::read_text_file(path);
    assert(file.has_value());
    auto const filename = path.string();
    try {
        auto const tokens = tokenize(filename, file.value());
        auto const statements = parse(tokens);
        execute(statements, executor);
    } catch (LexerError const& lexer_error) {
        std::cerr << lexer_error.what() << '\n';
    } catch (ParserError const& parser_error) {
        std::cerr << parser_error.what() << '\n';
    } catch (InterpreterError const& interpreter_error) {
        std::cerr << interpreter_error.what() << '\n';
    } catch (std::exception const& e) {
        std::cerr << "unexpected error: " << e.what() << '\n';
    }
}

void interpret(std::filesystem::path const& path) {
    auto interpreter = interpreter::Interpreter{};
    try_tokenize_parse_execute(path, interpreter);
}

void compile(std::filesystem::path const& path) {
    using namespace std::string_view_literals;

    auto compiler = compiler::Compiler{};
    try_tokenize_parse_execute(path, compiler);
    auto const llvm_ir = compiler.emit();
    static constexpr auto out_filename = "test.ll";
    auto const write_result = utils::write_text_file(out_filename, llvm_ir);
    assert(write_result.has_value());
    static constexpr auto arguments = std::array{
        "clang"sv, "-Wno-override-module"sv, "-o"sv, "test.exe"sv, std::string_view{ out_filename },
    };
    auto const&& [status, error_code] = reproc::run(arguments);
    if (error_code) {
        std::cerr << "invoking clang failed: process returned " << error_code.value() << '\n'
                  << error_code.message() << '\n';
    } else {
        std::cout << "clang returned: " << status << '\n';
    }
}
