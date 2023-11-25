#pragma once

#include "../executor.hpp"

namespace interpreter {
    class Interpreter final : public Executor {
    public:
        void print(statements::Print const& println) override;
        void println(statements::Println const& println) override;
    };
} // namespace interpreter
