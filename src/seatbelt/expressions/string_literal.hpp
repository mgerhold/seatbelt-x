#pragma once

#include "expression.hpp"

namespace expressions {

    class StringLiteral final : public BasicExpression {
    private:
        Token m_token;

    public:
        explicit StringLiteral(Token const& token) : m_token{ token } { }

        [[nodiscard]] Optional<StringLiteral const&> as_string_literal() const override {
            return *this;
        }

        [[nodiscard]] std::u8string unescaped() const;
    };

} // namespace expressions
