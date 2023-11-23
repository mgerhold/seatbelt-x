#include "interpreter.hpp"

#include "expressions/u32_literal.hpp"
#include "interpreter_error.hpp"
#include "statements/println.hpp"
#include "statements/statement.hpp"
#include <iostream>

namespace interpreter {

    void Interpreter::println(statements::Println const& println) {
        if (auto const& u32_literal = println.expression()->as_u32_literal()) {
            std::cout << u32_literal.value().token().location.ascii_lexeme() << '\n';
        } else {
            throw InvalidArgumentType{ println.token() };
        }
    }

} // namespace interpreter
