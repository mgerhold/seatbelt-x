#pragma once

#include "tokens.hpp"

#include <concepts>
#include <format>
#include <stdexcept>

class ParserError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class UnexpectedToken final : public ParserError {
public:
    explicit UnexpectedToken(Token const& actual, std::convertible_to<TokenType> auto&&... expected)
        : ParserError{ std::format(
                  "{}: expected token type '{}', got '{}' instead",
                  actual.location,
                  join(std::forward<decltype(expected)>(expected)...),
                  actual.type
          ) } {
        static_assert(sizeof...(expected) > 0);
    }

private:
    // clang-format off
    [[nodiscard]] static std::string join(
        std::convertible_to<TokenType> auto&& first,
        std::convertible_to<TokenType> auto&&... rest
    ) { // clang-format on
        if constexpr (sizeof...(rest) == 0) {
            return std::format("{}", first);
        } else {
            return std::format("{}, {}", first, join(std::forward<decltype(rest)>(rest)...));
        }
    }
};
