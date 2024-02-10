#pragma once

#include "types.hpp"
#include <filesystem>
#include <fstream>
#include <string>

namespace utils {

    enum class IoError {
        CouldNotOpenFile,
        UnableToReadFile,
        UnableToDetermineFileSize,
        UnableToWriteToFile,
    };

    [[nodiscard]] Result<void, IoError> write_text_file(std::filesystem::path const& path, std::string_view contents);
    [[nodiscard]] Result<std::u8string, IoError> read_text_file(std::filesystem::path const& path);

    enum class Utf8Error {
        InvalidUtf8String,
        InvalidUtf8Codepoint,
    };

    [[nodiscard]] Result<usize, Utf8Error> utf8_width(std::u8string_view string);
    [[nodiscard]] Result<std::u8string_view, Utf8Error> first_utf8_codepoint(std::u8string_view string);

    [[nodiscard]] std::string_view to_string_view(std::u8string_view string);

    template<std::integral T>
    [[nodiscard]] T to_integer(std::string string) {
        auto stream = std::stringstream{ std::move(string) };
        auto result = T{};
        stream >> result;
        if (not stream or not stream.eof()) {
            // todo: create specific exception type
            throw std::runtime_error{ "unable to convert string to integer" };
        }
        return result;
    }

} // namespace utils
