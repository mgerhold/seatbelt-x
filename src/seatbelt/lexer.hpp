#pragma once

#include "tokens.hpp"
#include "types.hpp"

using TokenVector = std::vector<Token>;

[[nodiscard]] TokenVector tokenize(std::string_view filename, std::u8string_view source_code);
