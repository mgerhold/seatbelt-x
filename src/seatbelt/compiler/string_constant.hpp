#pragma once

#include "../types.hpp"
#include <string>

namespace compiler {

    struct StringConstant final {
        std::string string;
        usize size;
    };

} // namespace compiler
