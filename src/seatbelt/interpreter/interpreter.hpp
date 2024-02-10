#pragma once

#include "../executor.hpp"
#include <iostream>

namespace interpreter {
    class Interpreter final : public Executor {
    private:
        std::ostream* m_std_out;

    public:
        explicit Interpreter(std::ostream* const m_stdout) : m_std_out(m_stdout) { }

        void print(statements::Print const& println) override;
        void println(statements::Println const& println) override;
    };
} // namespace interpreter
