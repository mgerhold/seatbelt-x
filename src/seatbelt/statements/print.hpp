#pragma once

#include "statement.hpp"

namespace statements {

    class Print final : public BasicStatement {
    private:
        Token m_token;
        expressions::Expression m_expression;

    public:
        Print(Token const& token, expressions::Expression expression)
            : m_token{ token },
              m_expression{ std::move(expression) } { }

        [[nodiscard]] expressions::Expression const& expression() const {
            return m_expression;
        }

        [[nodiscard]] Token const& token() const {
            return m_token;
        }

        [[nodiscard]] Optional<Print const&> as_print() const override {
            return *this;
        }
    };

} // namespace statements
