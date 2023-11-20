#include "include/seatbelt.hpp"
#include "lexer.hpp"
#include <iostream>
#include <magic_enum.hpp>

void compile(std::filesystem::path const& path) {
    auto const file = utils::read_text_file(path);
    assert(file.has_value());
    auto const filename = path.string();
    auto const tokens = tokenize(filename, file.value());
    if (not tokens.has_value()) {
        std::cerr << std::format(
                "{}:{}:{}: {}\n",
                tokens.error().location.filename(),
                tokens.error().location.line_number(),
                tokens.error().location.column_number(),
                magic_enum::enum_name(tokens.error().error_code)
        );
        return;
    }

    for (auto const& token : tokens.value()) {
        std::cout << magic_enum::enum_name(token.type) << " ('" << token.location.ascii_lexeme() << "')\n";
    }
}
