#include "parser.hpp"

#include "parser_error.hpp"
#include "statements/statement.hpp"
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
            case TokenType::Println:
                return println();
            default:
                throw std::runtime_error{ "replace me with a better exception type!" };
        }
    }

    [[nodiscard]] statements::Statement println() {
        advance(); // consume "println"
        expect(TokenType::LeftParenthesis);
        auto expression = this->expression();
        expect(TokenType::RightParenthesis);
        expect(TokenType::Semicolon);
        return std::make_unique<statements::Println>(std::move(expression));
    }

    [[nodiscard]] expressions::Expression expression() {
        return primary();
    }

    [[nodiscard]] expressions::Expression primary() {
        switch (current().type) {
            case TokenType::U32Literal:
                return std::make_unique<expressions::U32Literal>(advance());
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
