#pragma once

#include "tokens.hpp"


#include <stdexcept>

class InterpreterError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class InvalidArgumentType final : public InterpreterError {
public:
    explicit InvalidArgumentType(Token const& function)
        : InterpreterError{ std::format("{}: invalid argument type", function.location) } { }
};
