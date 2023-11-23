#pragma once
#include "../expressions/expression.hpp"


#include <memory>

namespace statements {
    class BasicStatement {
    public:
        virtual ~BasicStatement() = default;
    };

    class Println final : public BasicStatement {
    private:
        expressions::Expression m_expression;

    public:
        explicit Println(expressions::Expression expression) : m_expression{ std::move(expression) } { }
    };

    using Statement = std::unique_ptr<BasicStatement>;
} // namespace statements
