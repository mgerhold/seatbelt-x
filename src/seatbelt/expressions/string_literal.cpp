#include "string_literal.hpp"
#include <array>
#include <gsl/gsl>
#include <iostream>
#include <utf8proc.h>

namespace expressions {

    [[nodiscard]] std::u8string StringLiteral::unescaped() const {
        using namespace std::string_literals;
        assert(m_token.lexeme().size() >= 2); // includes at least two quotes
        assert(m_token.lexeme().front() == '"');
        assert(m_token.lexeme().back() == '"');
        auto result = u8""s;
        auto current = reinterpret_cast<utf8proc_uint8_t const*>(m_token.lexeme().data() + 1);
        auto const end =
                reinterpret_cast<utf8proc_uint8_t const*>(m_token.lexeme().data() + m_token.lexeme().size() - 1);
        auto first = utf8proc_int32_t{};
        auto second = utf8proc_int32_t{};
        while (current < end) {
            auto codepoint = utf8proc_int32_t{};
            auto const num_bytes_read = utf8proc_iterate(current, -1, &codepoint);
            if (codepoint == -1) {
                // todo: replace with better exception type
                throw std::runtime_error{ "string literal contains invalid UTF-8" };
            }
            assert(utf8proc_codepoint_valid(codepoint));
            first = std::exchange(second, codepoint);
            if (first == u8'\\') {
                assert(second == u8'\\' or second == u8't' or second == u8'n' or second == u8'v' or second == u8'f'
                       or second == u8'r' or second == u8'0' or second == u8'"');
                switch (second) {
                    case u8'\\':
                        result += u8'\\';
                        break;
                    case u8't':
                        result += u8'\t';
                        break;
                    case u8'n':
                        result += u8'\n';
                        break;
                    case u8'v':
                        result += u8'\v';
                        break;
                    case u8'f':
                        result += u8'\f';
                        break;
                    case u8'r':
                        result += u8'\r';
                        break;
                    case u8'0':
                        result += u8'\0';
                        break;
                    case u8'"':
                        result += u8'\"';
                        break;
                    default:
                        std::unreachable();
                        break;
                }
                second = {}; // "clear" escape sequence since it has already been fully consumed
            } else if (second != u8'\\') {
                // no escape sequence
                auto buffer = std::array<utf8proc_uint8_t, 4>{};
                [[maybe_unused]] auto const num_bytes_written = utf8proc_encode_char(codepoint, buffer.data());
                assert(num_bytes_written == num_bytes_read);
                for (auto i = decltype(num_bytes_read){ 0 }; i < num_bytes_read; ++i) {
                    result += buffer.at(gsl::narrow<usize>(i));
                }
            }
            current += num_bytes_read;
        }
        return result;
    }

} // namespace expressions
