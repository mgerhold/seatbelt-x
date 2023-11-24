#pragma once

#include "../types.hpp"
#include "value.hpp"

namespace compiler {

    class U32Constant final : public BasicValue {
    private:
        u32 m_value;

    public:
        explicit U32Constant(u32 const value) : m_value{ value } { }

        std::ostream& format(std::ostream& os) const override {
            return os << "i32 noundef " << m_value;
        }
    };

} // namespace compiler
