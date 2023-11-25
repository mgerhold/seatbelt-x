#include "function_call.hpp"
#include "function_declaration.hpp"

namespace compiler {

    std::ostream& operator<<(std::ostream& os, FunctionCall const& call) {
        auto const& function = call.m_callee.dereference();
        os << "call " << function.return_type() << "(";
        for (usize i = 0; i < function.parameter_types().size(); ++i) {
            os << function.parameter_types().at(i);
            if (i < function.parameter_types().size() - 1 or function.is_variadic()) {
                os << ", ";
            }
        }
        if (function.is_variadic()) {
            os << "...";
        }
        os << ") @" << function.name() << "(";
        for (usize i = 0; i < call.m_arguments.size(); ++i) {
            os << call.m_arguments.at(i);
            if (i < call.m_arguments.size() - 1) {
                os << ", ";
            }
        }
        return os << ")";
    }

} // namespace compiler
