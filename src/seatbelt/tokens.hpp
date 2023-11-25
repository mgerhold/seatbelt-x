#pragma once

#include "source_location.hpp"
#include <string_view>

enum class TokenType {
    And,
    Arrow,
    Asterisk,
    At,
    BoolLiteral,
    Break,
    CapitalizedFunction,
    CharLiteral,
    StringLiteral,
    Colon,
    Comma,
    Const,
    Continue,
    Do,
    Dot,
    DoubleColon,
    DumpRegisters,
    Else,
    EndOfFile,
    Equals,
    EqualsEquals,
    ExclamationMark,
    ExclamationMarkEquals,
    Export,
    For,
    ForwardSlash,
    Function,
    Goto,
    GreaterOrEquals,
    GreaterThan,
    Identifier,
    If,
    Import,
    InlineBssembly,
    Label,
    LeftCurlyBracket,
    LeftParenthesis,
    LeftSquareBracket,
    LessOrEquals,
    LessThan,
    Let,
    Loop,
    Minus,
    Mod,
    Mutable,
    Namespace,
    Not,
    NothingLiteral,
    Or,
    Plus,
    Restricted,
    Return,
    RightCurlyBracket,
    RightParenthesis,
    RightSquareBracket,
    Semicolon,
    Struct,
    TildeArrow,
    Type,
    TypeSize,
    U32Literal,
    ValueSize,
    While,
    Xor,
    Print,
    Println,
};

std::ostream& operator<<(std::ostream& os, TokenType type);

template<>
struct std::formatter<TokenType> : OStreamFormatter { };

struct Token {
    Token(SourceLocation const& location_, TokenType const type_) : location{ location_ }, type{ type_ } { }

    [[nodiscard]] std::u8string_view lexeme() const {
        return location.lexeme();
    }

    SourceLocation location;
    TokenType type;
};
