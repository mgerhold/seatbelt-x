#pragma once

#include "../executor.hpp"
#include "context.hpp"
#include "function_declaration.hpp"
#include "function_declaration_reference.hpp"
#include "string_constant.hpp"
#include "string_constant_reference.hpp"
#include "types.hpp"
#include <string>
#include <vector>

namespace compiler {

    class Compiler final : public Executor, public Context {
    private:
        std::vector<StringConstant> m_string_constants;
        std::vector<FunctionDeclaration> m_function_declarations;
        std::unordered_map<std::string, FunctionDeclarationReference> m_function_references;
        std::string m_main_body;
        usize m_next_id = 1;

    public:
        Compiler();
        Compiler(Compiler const& other) = delete;
        Compiler(Compiler&& other) noexcept = delete;
        Compiler& operator=(Compiler const& other) = delete;
        Compiler& operator=(Compiler&& other) noexcept = delete;

        void print(statements::Print const& println) override;
        void println(statements::Println const& println) override;
        [[nodiscard]] std::string emit();

        // clang-format off
        [[nodiscard]] FunctionDeclaration const& retrieve_function(
            FunctionDeclarationReference const& reference
        ) const override { // clang-format on
            return m_function_declarations.at(reference.m_id);
        }

    private:
        [[nodiscard]] Value add_string_constant(std::string_view string);

        [[nodiscard]] FunctionDeclarationReference declare_function(
                DataType return_type,
                std::string name,
                std::vector<DataType> parameter_types,
                bool is_variadic
        );

        [[nodiscard]] usize next_id() {
            return m_next_id++;
        }
    };

} // namespace compiler
