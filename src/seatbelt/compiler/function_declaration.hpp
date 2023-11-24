#pragma once

#include "../ostream_formatter.hpp"
#include "../types.hpp"
#include "context.hpp"
#include "data_type.hpp"
#include <format>
#include <iostream>
#include <string>
#include <vector>

namespace compiler {
    class FunctionCall;

    class FunctionDeclaration final {
        friend class FunctionCall;

    private:
        DataType m_return_type;
        std::string m_name;
        std::vector<DataType> m_parameter_types;
        bool m_is_variadic;

    public:
        FunctionDeclaration(
                DataType const return_type,
                std::string name,
                std::vector<DataType> parameter_types,
                bool const is_variadic
        )
            : m_return_type{ return_type },
              m_name{ std::move(name) },
              m_parameter_types{ std::move(parameter_types) },
              m_is_variadic{ is_variadic } { }

        [[nodiscard]] DataType return_type() const {
            return m_return_type;
        }

        [[nodiscard]] std::string const& name() const {
            return m_name;
        }

        [[nodiscard]] std::vector<DataType> const& parameter_types() const {
            return m_parameter_types;
        }

        [[nodiscard]] bool is_variadic() const {
            return m_is_variadic;
        }

        friend std::ostream& operator<<(std::ostream& os, FunctionDeclaration const& declaration) {
            os << "declare " << declaration.m_return_type << " @" << declaration.m_name << '(';
            for (usize i = 0; i < declaration.m_parameter_types.size(); ++i) {
                os << declaration.m_parameter_types.at(i) << " noundef";
                if (i < declaration.m_parameter_types.size() - 1 or declaration.m_is_variadic) {
                    os << ", ";
                }
            }
            if (declaration.m_is_variadic) {
                os << "...";
            }

            return os << ')';
        }
    };

} // namespace compiler


template<>
struct std::formatter<compiler::FunctionDeclaration> : OStreamFormatter { };
