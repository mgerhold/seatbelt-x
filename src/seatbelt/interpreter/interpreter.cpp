#include "interpreter.hpp"

#include "../expressions/string_literal.hpp"
#include "../expressions/u32_literal.hpp"
#include "../statements/print.hpp"
#include "../statements/println.hpp"
#include "../statements/statement.hpp"
#include "interpreter_error.hpp"
#include <iostream>

static void print_implementation(expressions::Expression const& expression, Token print_token, std::ostream& ostream) {
    if (auto const& u32_literal = expression->as_u32_literal()) {
        ostream << u32_literal.value().token().location.ascii_lexeme();
    } else if (auto const& string_literal = expression->as_string_literal()) {
        ostream << utils::to_string_view(std::u8string_view{ string_literal.value().unescaped() });
    } else {
        throw InvalidArgumentType{ print_token };
    }
}

namespace interpreter {

    void Interpreter::print(statements::Print const& print) {
        print_implementation(print.expression(), print.token(), *m_std_out);
    }

    void Interpreter::println(statements::Println const& println) {
        print_implementation(println.expression(), println.token(), *m_std_out);
        *m_std_out << '\n';
    }

} // namespace interpreter
