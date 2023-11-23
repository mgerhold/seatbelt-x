#pragma once

#include "../expressions/expression.hpp"
#include <memory>

namespace statements {
    class Print;
    class Println;
} // namespace statements

namespace statements {
    class BasicStatement {
    public:
        virtual ~BasicStatement() = default;

        [[nodiscard]] virtual Optional<Print const&> as_print() const {
            return none;
        }

        [[nodiscard]] virtual Optional<Println const&> as_println() const {
            return none;
        }
    };

    using Statement = std::unique_ptr<BasicStatement>;
} // namespace statements
