#include "lexer.hpp"


#include "lexer_error.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <ctre-unicode.hpp>
#include <utility>

using namespace std::string_view_literals;

static constexpr char char_pattern[] = R"('(\\'|[ -\[\]-~]|\\[n\\tnvfr0])')";
static constexpr char string_pattern[] = R"("(\\"|\\[\\tnvfr0]|[^\\"])*")";
static constexpr char integer_pattern[] = "(0o([0-7]+_?)+)|(0x([\\dA-Fa-f]+_?)+)|(0b([01]+_?)+)|(\\d+_?)+";
static constexpr char identifier_pattern[] = R"(\p{XID_Start}\p{XID_Continue}*)";

static constexpr auto non_keyword_tokens = std::array{
    std::pair{ u8"->"sv,                 TokenType::Arrow },
    std::pair{ u8"~>"sv,            TokenType::TildeArrow },
    std::pair{ u8"!="sv, TokenType::ExclamationMarkEquals },
    std::pair{ u8"::"sv,           TokenType::DoubleColon },
    std::pair{ u8">="sv,       TokenType::GreaterOrEquals },
    std::pair{ u8"<="sv,          TokenType::LessOrEquals },
    std::pair{ u8"=="sv,          TokenType::EqualsEquals },
    std::pair{  u8"-"sv,                 TokenType::Minus },
    std::pair{  u8":"sv,                 TokenType::Colon },
    std::pair{  u8","sv,                 TokenType::Comma },
    std::pair{  u8";"sv,             TokenType::Semicolon },
    std::pair{  u8"+"sv,                  TokenType::Plus },
    std::pair{  u8"-"sv,                 TokenType::Minus },
    std::pair{  u8"*"sv,              TokenType::Asterisk },
    std::pair{  u8"/"sv,          TokenType::ForwardSlash },
    std::pair{  u8"("sv,       TokenType::LeftParenthesis },
    std::pair{  u8")"sv,      TokenType::RightParenthesis },
    std::pair{  u8"{"sv,      TokenType::LeftCurlyBracket },
    std::pair{  u8"}"sv,     TokenType::RightCurlyBracket },
    std::pair{  u8"["sv,     TokenType::LeftSquareBracket },
    std::pair{  u8"]"sv,    TokenType::RightSquareBracket },
    std::pair{  u8"="sv,                TokenType::Equals },
    std::pair{  u8"."sv,                   TokenType::Dot },
    std::pair{  u8"!"sv,       TokenType::ExclamationMark },
    std::pair{  u8">"sv,           TokenType::GreaterThan },
    std::pair{  u8"<"sv,              TokenType::LessThan },
    std::pair{  u8"@"sv,                    TokenType::At },
};


static constexpr auto keywords = std::array{
    std::pair{ u8"dump_registers"sv,       TokenType::DumpRegisters },
    std::pair{       u8"function"sv,            TokenType::Function },
    std::pair{            u8"mod"sv,                 TokenType::Mod },
    std::pair{            u8"let"sv,                 TokenType::Let },
    std::pair{           u8"true"sv,         TokenType::BoolLiteral },
    std::pair{          u8"false"sv,         TokenType::BoolLiteral },
    std::pair{            u8"bsm"sv,      TokenType::InlineBssembly },
    std::pair{         u8"import"sv,              TokenType::Import },
    std::pair{      u8"namespace"sv,           TokenType::Namespace },
    std::pair{            u8"and"sv,                 TokenType::And },
    std::pair{             u8"or"sv,                  TokenType::Or },
    std::pair{            u8"not"sv,                 TokenType::Not },
    std::pair{            u8"xor"sv,                 TokenType::Xor },
    std::pair{             u8"if"sv,                  TokenType::If },
    std::pair{           u8"else"sv,                TokenType::Else },
    std::pair{           u8"loop"sv,                TokenType::Loop },
    std::pair{          u8"break"sv,               TokenType::Break },
    std::pair{       u8"continue"sv,            TokenType::Continue },
    std::pair{          u8"while"sv,               TokenType::While },
    std::pair{             u8"do"sv,                  TokenType::Do },
    std::pair{            u8"for"sv,                 TokenType::For },
    std::pair{        u8"mutable"sv,             TokenType::Mutable },
    std::pair{          u8"const"sv,               TokenType::Const },
    std::pair{         u8"return"sv,              TokenType::Return },
    std::pair{          u8"label"sv,               TokenType::Label },
    std::pair{           u8"goto"sv,                TokenType::Goto },
    std::pair{        u8"nothing"sv,      TokenType::NothingLiteral },
    std::pair{       u8"Function"sv, TokenType::CapitalizedFunction },
    std::pair{         u8"export"sv,              TokenType::Export },
    std::pair{      u8"type_size"sv,            TokenType::TypeSize },
    std::pair{     u8"value_size"sv,           TokenType::ValueSize },
    std::pair{           u8"type"sv,                TokenType::Type },
    std::pair{         u8"struct"sv,              TokenType::Struct },
    std::pair{     u8"restricted"sv,          TokenType::Restricted },
    std::pair{          u8"print"sv,               TokenType::Print },
    std::pair{        u8"println"sv,             TokenType::Println },
};

struct LexerState final {
private:
    std::string_view m_filename;
    std::u8string_view m_source_code;
    usize m_index{ 0 };
    TokenVector tokens{};

public:
    LexerState(std::string_view const filename, std::u8string_view const sourceCode)
        : m_filename{ filename },
          m_source_code{ sourceCode } { }

    [[nodiscard]] char8_t current() const {
        return m_source_code.at(m_index);
    }

    [[nodiscard]] Optional<char8_t> peek(usize const offset = 1) const {
        if (m_index + offset >= m_source_code.length()) {
            return {};
        }
        return m_source_code[m_index + offset];
    }

    void advance_bytes(usize const amount = 1) {
        m_index += amount;
    }

    [[nodiscard]] bool is_end_of_file() const {
        return m_index >= m_source_code.length();
    }

    [[nodiscard]] auto filename() const {
        return m_filename;
    }

    [[nodiscard]] auto source_code() const {
        return m_source_code;
    }

    [[nodiscard]] auto source_location_from_bytes(usize const num_bytes = 1) const {
        assert(not is_end_of_file());
        auto const lexeme = std::u8string_view{ &m_source_code.at(m_index), &m_source_code.at(m_index) + num_bytes };
        return SourceLocation{ m_filename, m_source_code, lexeme };
    }

    [[nodiscard]] auto source_location_from_codepoints(usize const num_codepoints = 1) const {
        assert(not is_end_of_file());
        auto current_index = m_index;
        for (usize i = 0; i < num_codepoints; ++i) {
            auto const remaining_source = m_source_code.substr(m_index);
            auto const codepoint = utils::first_utf8_codepoint(remaining_source).value();
            current_index += codepoint.length();
        }
        auto const lexeme = std::u8string_view{ &m_source_code.at(m_index), &m_source_code.at(current_index) };
        return SourceLocation{ m_filename, m_source_code, lexeme };
    }

    [[nodiscard]] auto view_from_current() const {
        assert(not is_end_of_file());
        return m_source_code.substr(m_index);
    }

    [[nodiscard]] bool current_is_valid_codepoint() const {
        auto const codepoint = utils::first_utf8_codepoint(view_from_current());
        return codepoint.has_value();
    }

    void push_token(TokenType const token_type, usize const num_lexeme_bytes = 1) {
        tokens.emplace_back(source_location_from_bytes(num_lexeme_bytes), token_type);
        advance_bytes(num_lexeme_bytes);
    }

    [[nodiscard]] TokenVector&& tokens_moved() {
        // first add end of file token
        tokens.emplace_back(
                SourceLocation{ filename(), source_code(), source_code().substr(source_code().length() - 1) },
                TokenType::EndOfFile
        );

        return std::move(tokens);
    }

    [[nodiscard]] bool try_consume_whitespace() {
        if (std::isspace(static_cast<unsigned char>(current()))) {
            advance_bytes();
            return true;
        }
        return false;
    }

    [[nodiscard]] bool try_consume_single_line_comment() {
        if (auto const next = peek(); next and current() == '/' and *next == '/') {
            advance_bytes(2);
            while (not is_end_of_file() and current() != '\n') {
                advance_bytes();
            }
            return true;
        }
        return false;
    }

    [[nodiscard]] bool try_consume_multiline_comment() {
        if (auto next = peek(); next and current() == '/' and *next == '*') {
            auto const starting_source_location = source_location_from_bytes(2);
            advance_bytes(2);
            auto nesting_level = usize{ 1 };

            while (not is_end_of_file()) {
                if (next = peek(); next and current() == '/' and *next == '*') {
                    advance_bytes(2);
                    ++nesting_level;
                    continue;
                }

                if (next = peek(); next and current() == '*' and *next == '/') {
                    advance_bytes(2);
                    --nesting_level;
                    if (nesting_level == 0) {
                        break;
                    }
                    continue;
                }

                advance_bytes();
            }
            if (is_end_of_file()) {
                throw UnterminatedComment{ starting_source_location };
            }
            return true;
        }
        return false;
    }

    [[nodiscard]] bool try_consume_non_keyword_token() {
        auto const non_keyword_iterator = std::ranges::find_if(non_keyword_tokens, [&](auto const& pair) {
            const auto& lexeme = pair.first;
            return view_from_current().starts_with(lexeme);
        });
        auto const found_non_keyword_token = (non_keyword_iterator != non_keyword_tokens.cend());
        if (found_non_keyword_token) {
            auto const& [lexeme, token_type] = *non_keyword_iterator;
            push_token(token_type, lexeme.length());
            return true;
        }
        return false;
    }

    [[nodiscard]] bool try_consume_char_literal() {
        if (auto const char_literal_result = ctre::starts_with<char_pattern>(view_from_current())) {
            auto const length = char_literal_result.view().length();
            push_token(TokenType::CharLiteral, length);
            return true;
        }
        return false;
    }

    [[nodiscard]] bool try_consume_string_literal() {
        if (auto const string_literal_result = ctre::starts_with<string_pattern>(view_from_current())) {
            auto const length = string_literal_result.view().length();
            push_token(TokenType::StringLiteral, length);
            return true;
        }
        return false;
    }

    [[nodiscard]] bool try_consume_u32_literal() {
        if (auto const integer_literal_result = ctre::starts_with<integer_pattern>(view_from_current())) {
            auto const length = integer_literal_result.view().length();
            push_token(TokenType::U32Literal, length);
            return true;
        }
        return false;
    }

    [[nodiscard]] bool try_consume_identifier_or_keyword() {
        if (auto const identifier_result = ctre::starts_with<identifier_pattern>(view_from_current())) {
            auto const keyword_iterator = std::ranges::find_if(keywords, [&](auto const& pair) {
                return pair.first == identifier_result.view();
            });
            auto const is_keyword = (keyword_iterator != keywords.cend());
            if (is_keyword) {
                auto const& [lexeme, token_type] = *keyword_iterator;
                push_token(token_type, lexeme.length());
            } else {
                auto const length = identifier_result.view().length();
                push_token(TokenType::Identifier, length);
            }
            return true;
        }
        return false;
    }
};


[[nodiscard]] TokenVector tokenize(std::string_view const filename, std::u8string_view const source_code) {
    assert(not source_code.empty() and source_code.back() == '\n');

    auto state = LexerState{ filename, source_code };

    while (not state.is_end_of_file()) {
        if (not state.current_is_valid_codepoint()) {
            throw InvalidUtf8Codepoint{ state.source_location_from_bytes() };
        }

        if (state.try_consume_whitespace() or state.try_consume_single_line_comment()
            or state.try_consume_multiline_comment() or state.try_consume_non_keyword_token()
            or state.try_consume_char_literal() or state.try_consume_u32_literal()
            or state.try_consume_string_literal()
            or state.try_consume_identifier_or_keyword()) {
            continue;
        }

        throw InvalidInput{ state.source_location_from_codepoints() };
    }

    return state.tokens_moved();
}
