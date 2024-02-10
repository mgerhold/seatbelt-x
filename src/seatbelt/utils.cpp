#include "utils.hpp"
#include <cassert>
#include <gsl/gsl>
#include <utf8proc.h>

namespace utils {

    // clang-format off
    [[nodiscard]] Result<void, IoError> write_text_file(
        std::filesystem::path const& path,
        std::string_view const contents
    ) { // clang-format on
        auto file = std::ofstream{ path };
        if (not file) {
            return Error<IoError>{ IoError::CouldNotOpenFile };
        }
        file << contents;
        if (not file) {
            return Error<IoError>{ IoError::UnableToWriteToFile };
        }
        return {};
    }

    [[nodiscard]] Result<std::u8string, IoError> read_text_file(std::filesystem::path const& path) {
        // open file with file cursor at the end, binary to not have windows newlines replaced
        auto file = std::ifstream{ path, std::ios::in | std::ios::binary | std::ios::ate };
        if (not file) {
            return Error<IoError>{ IoError::CouldNotOpenFile };
        }

        // determine filesize
        auto const tellg_result = file.tellg();
        if (tellg_result == decltype(tellg_result){ -1 }) {
            return Error<IoError>{ IoError::UnableToDetermineFileSize };
        }
        auto const size = gsl::narrow_cast<usize>(tellg_result);

        // reset file cursor to the beginning
        file.seekg(0, std::ios::beg);

        // read contents of the file
        auto result = std::u8string{};
        result.resize(size + 1);
        try {
            file.read(reinterpret_cast<char*>(result.data()), gsl::narrow_cast<std::streamsize>(size));
        } catch (std::ios::failure const&) {
            return Error<IoError>{ IoError::UnableToReadFile };
        }

        // always end the string with a newline
        result.back() = '\n';

        return result;
    }

    [[nodiscard]] Result<usize, Utf8Error> utf8_width(std::u8string_view const string) {
        auto width = usize{ 0 };
        auto current = reinterpret_cast<utf8proc_uint8_t const*>(string.data());
        auto codepoint = utf8proc_int32_t{};

        while (current < reinterpret_cast<utf8proc_uint8_t const*>(string.data() + string.length())) {
            auto const bytes_read = utf8proc_iterate(current, -1, &codepoint);
            if (codepoint == -1) {
                return Error<Utf8Error>{ Utf8Error::InvalidUtf8String };
            }
            assert(utf8proc_codepoint_valid(codepoint));
            width += static_cast<usize>(utf8proc_charwidth(codepoint));
            current += bytes_read;
        }
        if (current != reinterpret_cast<utf8proc_uint8_t const*>(string.data() + string.length())) {
            return Error<Utf8Error>{ Utf8Error::InvalidUtf8String };
        }
        return width;
    }

    [[nodiscard]] Result<std::u8string_view, Utf8Error> first_utf8_codepoint(std::u8string_view const string) {
        auto current = reinterpret_cast<utf8proc_uint8_t const*>(string.data());
        auto codepoint = utf8proc_int32_t{};
        auto bytes_read = utf8proc_iterate(current, -1, &codepoint);
        if (codepoint == -1) {
            return Error<Utf8Error>{ Utf8Error::InvalidUtf8Codepoint };
        }
        return string.substr(0, gsl::narrow<usize>(bytes_read));
    }

    [[nodiscard]] std::string_view to_string_view(std::u8string_view const string) {
        auto const view = std::string_view{
            reinterpret_cast<char const*>(string.data()),
            reinterpret_cast<char const*>(string.data()) + string.length(),
        };
        return view;
    }

} // namespace utils
