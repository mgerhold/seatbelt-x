#pragma once

#include "statements/statement.hpp"
#include <span>

[[nodiscard]] std::vector<statements::Statement> parse(std::span<Token const> tokens);
