#include "include/seatbelt.hpp"

#include "executor.hpp"
#include "interpreter/interpreter.hpp"
#include "interpreter_error.hpp"
#include "lexer.hpp"
#include "lexer_error.hpp"
#include "parser.hpp"
#include "parser_error.hpp"

#include <iostream>
#include <magic_enum.hpp>

static void execute(std::vector<statements::Statement> const& statements, Executor& executor) {
    for (auto const& statement : statements) {
        if (auto const& println = statement->as_println()) {
            executor.println(println.value());
        } else if (auto const& print = statement->as_print()) {
            executor.print(print.value());
        }
    }
}

void compile(std::filesystem::path const& path) {
    auto const file = utils::read_text_file(path);
    assert(file.has_value());
    auto const filename = path.string();
    try {
        auto const tokens = tokenize(filename, file.value());
        for (auto const& token : tokens) {
            std::cout << magic_enum::enum_name(token.type) << " ('" << token.location.ascii_lexeme() << "')\n";
        }
        auto const statements = parse(tokens);
        auto executor = interpreter::Interpreter{};
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
