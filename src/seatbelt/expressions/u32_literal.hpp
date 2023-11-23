#pragma once

#include "expression.hpp"

namespace expressions {
    class U32Literal final : public BasicExpression {
    private:
        Token m_token;

    public:
        explicit U32Literal(Token const& token) : m_token{ token } { }

        [[nodiscard]] Token const& token() const {
            return m_token;
        }

        [[nodiscard]] Optional<U32Literal const&> as_u32_literal() const override {
            return *this;
        }
    };
} // namespace expressions
