#pragma once

#include "../ostream_formatter.hpp"
#include "context.hpp"
#include "function_declaration_reference.hpp"
#include "value.hpp"
#include <format>
#include <iostream>

namespace compiler {

    class FunctionCall final {
    private:
        FunctionDeclarationReference m_callee;
        std::vector<Value> m_arguments;

    public:
        FunctionCall(FunctionDeclarationReference const callee, std::vector<Value> arguments)
            : m_callee{ callee },
              m_arguments{ std::move(arguments) } { }

        friend std::ostream& operator<<(std::ostream& os, FunctionCall const& call);
    };

} // namespace compiler

template<>
struct std::formatter<compiler::FunctionCall> : OStreamFormatter { };
