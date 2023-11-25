#pragma once

#include "../tokens.hpp"
#include <memory>

namespace expressions {
    class U32Literal;
    class StringLiteral;
}

namespace expressions {

    class BasicExpression {
    public:
        virtual ~BasicExpression() = default;

        [[nodiscard]] virtual Optional<U32Literal const&> as_u32_literal() const {
            return none;
        }

        [[nodiscard]] virtual Optional<StringLiteral const&> as_string_literal() const {
            return none;
        }
    };

    using Expression = std::unique_ptr<BasicExpression>;

} // namespace expressions
