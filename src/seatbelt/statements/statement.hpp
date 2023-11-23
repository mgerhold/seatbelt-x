#pragma once

#include "../expressions/expression.hpp"
#include <memory>

namespace statements {
    class Println;
}

namespace statements {
    class BasicStatement {
    public:
        virtual ~BasicStatement() = default;

        [[nodiscard]] virtual Optional<Println const&> as_println() const {
            return none;
        }
    };

    using Statement = std::unique_ptr<BasicStatement>;
} // namespace statements
