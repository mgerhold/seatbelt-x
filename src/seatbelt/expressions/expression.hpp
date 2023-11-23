#pragma once

#include "../tokens.hpp"
#include "expression.hpp"
#include <memory>

namespace expressions {
    class U32Literal;
}

namespace expressions {

    class BasicExpression {
    public:
        virtual ~BasicExpression() = default;

        [[nodiscard]] virtual Optional<U32Literal const&> as_u32_literal() const {
            return none;
        }
    };

    using Expression = std::unique_ptr<BasicExpression>;

} // namespace expressions
