#pragma once
#include "../tokens.hpp"
#include "expression.hpp"


#include <memory>

namespace expressions {

    class BasicExpression {
    public:
        virtual ~BasicExpression() = default;
    };

    class U32Literal final : public BasicExpression {
    private:
        Token m_token;

    public:
        explicit U32Literal(Token const& token) : m_token{ token } { }
    };

    using Expression = std::unique_ptr<BasicExpression>;

} // namespace expressions
