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

TEST(InterpreterTests, Print) {
    static constexpr auto source = R"(println("test");
)"sv;
    write_text_file(source_filename, source);
    auto std_out = std::ostringstream{};
    interpret(source_filename, std_out);
    EXPECT_EQ("test\n"sv, std_out.str());
}
