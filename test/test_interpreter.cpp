#include <cassert>
#include <fstream>
#include <gtest/gtest.h>
#include <seatbelt.hpp>
#include <sstream>
#include <string_view>

using namespace std::string_view_literals;

static constexpr auto source_filename = "test.bs"sv;

static void write_text_file(std::filesystem::path const& path, std::string_view const contents) {
    auto file = std::ofstream{ path };
    assert(file);
    file << contents;
    assert(file);
}

static std::string interpret_source(std::string_view const source) {
    write_text_file(source_filename, source);
    auto std_out = std::ostringstream{};
    interpret(source_filename, std_out);
    return std::move(std_out).str();
}

TEST(InterpreterTests, Print) {
    EXPECT_EQ("test"sv, interpret_source("print(\"test\");\n"sv));
    EXPECT_EQ("\""sv, interpret_source("print(\"\\\"\");\n"sv));
    EXPECT_EQ("🦀"sv, interpret_source("print(\"🦀\");\n"sv));
    EXPECT_EQ("Hello,\nworld!"sv, interpret_source("print(\"Hello,\\nworld!\");\n"sv));
    EXPECT_EQ("42"sv, interpret_source("print(42);\n"sv));
    EXPECT_EQ(" "sv, interpret_source("print(\" \");\n"sv));
}

TEST(InterpreterTests, PrintLn) {
    EXPECT_EQ("test\n"sv, interpret_source("println(\"test\");\n"sv));
    EXPECT_EQ("\"\n"sv, interpret_source("println(\"\\\"\");\n"sv));
    EXPECT_EQ("🦀\n"sv, interpret_source("println(\"🦀\");\n"sv));
    EXPECT_EQ("Hello,\nworld!\n"sv, interpret_source("println(\"Hello,\\nworld!\");\n"sv));
    EXPECT_EQ("42\n"sv, interpret_source("println(42);\n"sv));
    EXPECT_EQ(" \n"sv, interpret_source("println(\" \");\n"sv));
}
