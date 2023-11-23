#pragma once

#include "executor.hpp"

namespace interpreter {
    class Interpreter final : public Executor {
    public:
        void println(statements::Println const& println) override;
    };
} // namespace interpreter
