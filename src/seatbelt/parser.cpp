#include "parser.hpp"
#include "expressions/string_literal.hpp"
#include "expressions/u32_literal.hpp"
#include "parser_error.hpp"
#include "statements/print.hpp"
#include "statements/println.hpp"
#include "tokens.hpp"

#include <span>

class ParserState final {
private:
    std::span<Token const> m_tokens;
    usize m_index = 0;
    std::vector<statements::Statement> m_statements;

public:
    explicit ParserState(std::span<Token const> const tokens) : m_tokens{ tokens } {
        assert(not tokens.empty());
        assert(tokens.back().type == TokenType::EndOfFile);
    }

    [[nodiscard]] std::vector<statements::Statement>&& move_statements_out() && {
        return std::move(m_statements);
    }

    void statements() {
        while (not is_at_end()) {
            m_statements.push_back(statement());
        }
    }

private:
    statements::Statement statement() {
        switch (current().type) {
            case TokenType::Print:
                return print();
            case TokenType::Println:
                return println();
            default:
                throw UnexpectedToken{ current(), TokenType::Println };
        }
    }

    [[nodiscard]] statements::Statement print() {
        auto print_token = advance();
        expect(TokenType::LeftParenthesis);
        auto expression = this->expression();
        expect(TokenType::RightParenthesis);
        expect(TokenType::Semicolon);
        return std::make_unique<statements::Print>(print_token, std::move(expression));
    }

    [[nodiscard]] statements::Statement println() {
        auto println_token = advance();
        expect(TokenType::LeftParenthesis);
        auto expression = this->expression();
        expect(TokenType::RightParenthesis);
        expect(TokenType::Semicolon);
        return std::make_unique<statements::Println>(println_token, std::move(expression));
    }

    [[nodiscard]] expressions::Expression expression() {
        return primary();
    }

    [[nodiscard]] expressions::Expression primary() {
        switch (current().type) {
            case TokenType::U32Literal:
                return std::make_unique<expressions::U32Literal>(advance());
            case TokenType::StringLiteral:
                return std::make_unique<expressions::StringLiteral>(advance());
            default:
                throw UnexpectedToken{ current(), TokenType::U32Literal };
        }
    }

    [[nodiscard]] bool is_at_end() const {
        return m_index >= m_tokens.size() or m_tokens[m_index].type == TokenType::EndOfFile;
    }

    [[nodiscard]] Token const& current() const {
        return is_at_end() ? m_tokens.back() : m_tokens[m_index];
    }

    [[nodiscard]] Token const& peek() const {
        if (m_index + 1 >= m_tokens.size() or m_tokens[m_index + 1].type == TokenType::EndOfFile) {
            return m_tokens.back();
        }
        return m_tokens[m_index + 1];
    }

    Token const& expect(std::convertible_to<TokenType> auto&&... types) {
        if (((current().type == types) or ...)) {
            return advance();
        }
        throw UnexpectedToken{ current(), std::forward<decltype(types)>(types)... };
    }

    Token const& advance() {
        auto const& consumed = current();
        if (not is_at_end()) {
            ++m_index;
        }
        return consumed;
    }
};

[[nodiscard]] std::vector<statements::Statement> parse(std::span<Token const> const tokens) {
    auto state = ParserState{ tokens };
    state.statements();
    return std::move(state).move_statements_out();
}
