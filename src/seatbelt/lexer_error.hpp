#pragma once

#include "source_location.hpp"

#include <format>
#include <stdexcept>

class LexerError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class UnterminatedComment final : public LexerError {
public:
    explicit UnterminatedComment(SourceLocation const& location)
        : LexerError{ std::format("{}: unterminated comment", location) } { }
};

class InvalidUtf8Codepoint final : public LexerError {
public:
    explicit InvalidUtf8Codepoint(SourceLocation const& location)
        : LexerError{ std::format("{}: invalid UTF 8 codepoint", location) } { }
};

class InvalidInput final : public LexerError {
public:
    explicit InvalidInput(SourceLocation const& location)
        : LexerError{ std::format("{}: invalid input", location) } { }
};