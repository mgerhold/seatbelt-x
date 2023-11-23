#include "include/seatbelt.hpp"
#include "lexer.hpp"
#include "lexer_error.hpp"

#include <iostream>
#include <magic_enum.hpp>

void compile(std::filesystem::path const& path) {
    auto const file = utils::read_text_file(path);
    assert(file.has_value());
    auto const filename = path.string();
    try {
        auto const tokens = tokenize(filename, file.value());
        for (auto const& token : tokens) {
            std::cout << magic_enum::enum_name(token.type) << " ('" << token.location.ascii_lexeme() << "')\n";
        }
    } catch (LexerError const& lexer_error) {
        std::cerr << "lexer error: " << lexer_error.what() << '\n';
    }
}
