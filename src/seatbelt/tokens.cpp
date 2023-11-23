#include "tokens.hpp"

#include <magic_enum.hpp>

std::ostream& operator<<(std::ostream& os, TokenType const type) {
    return os << magic_enum::enum_name(type);
}
