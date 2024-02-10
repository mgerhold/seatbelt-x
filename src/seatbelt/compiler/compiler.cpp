#include "compiler.hpp"


#include "../expressions/string_literal.hpp"
#include "../expressions/u32_literal.hpp"
#include "../statements/print.hpp"
#include "../statements/println.hpp"
#include "function_call.hpp"
#include "interpreter_error.hpp"
#include "u32_constant.hpp"

#include <format>
#include <gsl/gsl>

namespace compiler {

    Compiler::Compiler() {
        m_function_references.insert(
                { "printf", declare_function(DataType::I32, "printf", std::vector{ DataType::Pointer }, true) }
        );
    }

    void Compiler::print(statements::Print const& print) {
        auto const& printf = m_function_references.at("printf");
        if (auto const& u32_literal = print.expression()->as_u32_literal()) {
            auto format_string = add_string_constant("%u");
            auto arguments = std::vector<Value>{};
            arguments.push_back(std::move(format_string));
            arguments.push_back(std::make_unique<U32Constant>(utils::to_integer<u32>(std::string{
                    u32_literal.value().token().location.ascii_lexeme() })));
            m_main_body += std::format("  %{} = {}\n", next_id(), FunctionCall{ printf, std::move(arguments) });
        } else if (auto const& string_literal = print.expression()->as_string_literal()) {
            auto format_string = add_string_constant("%s");
            auto string_constant = add_string_constant(utils::to_string_view(string_literal->unescaped()));
            auto arguments = std::vector<Value>{};
            arguments.push_back(std::move(format_string));
            arguments.push_back(std::move(string_constant));
            m_main_body += std::format("  %{} = {}\n", next_id(), FunctionCall{ printf, std::move(arguments) });
        } else {
            throw InvalidArgumentType{ print.token() };
        }
    }

    void Compiler::println(statements::Println const& println) {
        auto const& printf = m_function_references.at("printf");
        if (auto const& u32_literal = println.expression()->as_u32_literal()) {
            auto format_string = add_string_constant("%u\n");
            auto arguments = std::vector<Value>{};
            arguments.push_back(std::move(format_string));
            arguments.push_back(std::make_unique<U32Constant>(utils::to_integer<u32>(std::string{
                    u32_literal.value().token().location.ascii_lexeme() })));
            m_main_body += std::format("  %{} = {}\n", next_id(), FunctionCall{ printf, std::move(arguments) });
        } else if (auto const& string_literal = println.expression()->as_string_literal()) {
            auto format_string = add_string_constant("%s\n");
            auto string_constant = add_string_constant(utils::to_string_view(string_literal->unescaped()));
            auto arguments = std::vector<Value>{};
            arguments.push_back(std::move(format_string));
            arguments.push_back(std::move(string_constant));
            m_main_body += std::format("  %{} = {}\n", next_id(), FunctionCall{ printf, std::move(arguments) });
        } else {
            throw InvalidArgumentType{ println.token() };
        }
    }

    [[nodiscard]] std::string Compiler::emit() {
        using namespace std::string_literals;
        auto result = ""s;

        // string constants
        for (usize i = 0; i < m_string_constants.size(); ++i) {
            auto const& [string, length] = m_string_constants.at(i);
            result += std::format(
                    R"(@.str{} = private unnamed_addr constant [{} x i8] c"{}\00", align 1
)",
                    i == 0 ? ""s : std::format(".{}", i),
                    length + 1,
                    string
            );
        }

        result += '\n';

        // main function
        result += "define dso_local i32 @main() {\n";
        result += m_main_body;
        result += "  ret i32 0\n";
        result += "}\n\n";

        // function declarations
        for (auto const& declaration : m_function_declarations) {
            result += std::format("{}\n", declaration);
        }

        return result;
    }

    [[nodiscard]] Value Compiler::add_string_constant(std::string_view const string) {
        auto encoded = std::string{};
        for (auto const c : string) {
            if (std::isprint(static_cast<unsigned char>(c)) and c != '\\' and c != '"') {
                encoded += c;
            } else {
                encoded += std::format("\\{:02X}", static_cast<unsigned char>(c));
            }
        }

        auto const find_iterator =
                std::find_if(std::cbegin(m_string_constants), std::cend(m_string_constants), [&](auto const& entry) {
                    return entry.string == encoded;
                });
        auto const found = (find_iterator != std::cend(m_string_constants));
        if (found) {
            return std::make_unique<StringConstantReference>(
                    gsl::narrow<usize>(std::distance(std::cbegin(m_string_constants), find_iterator))
            );
        }
        m_string_constants.emplace_back(encoded, string.length());
        return std::make_unique<StringConstantReference>(m_string_constants.size() - 1);
    }

    FunctionDeclarationReference Compiler::declare_function(
            DataType return_type,
            std::string name,
            std::vector<DataType> parameter_types,
            bool is_variadic
    ) {
        m_function_declarations.emplace_back(return_type, std::move(name), std::move(parameter_types), is_variadic);
        return FunctionDeclarationReference{ m_function_declarations.size() - 1, this };
    }

} // namespace compiler
