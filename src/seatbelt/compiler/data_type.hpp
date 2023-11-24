#pragma once

#include "../ostream_formatter.hpp"
#include <ostream>
#include <utility>

namespace compiler {

    enum class DataType {
        I32,
        Pointer,
    };

    inline std::ostream& operator<<(std::ostream& os, DataType const type) {
        switch (type) {
            case DataType::I32:
                return os << "i32";
            case DataType::Pointer:
                return os << "ptr";
        }
        std::unreachable();
    }

} // namespace compiler

template<>
struct std::formatter<compiler::DataType> : OStreamFormatter { };
