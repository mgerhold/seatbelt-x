#pragma once

#include "../types.hpp"
#include "ostream_formatter.hpp"
#include "value.hpp"

#include <ostream>

namespace compiler {

    class StringConstantReference final : public BasicValue {
    private:
        usize m_id;

    public:
        explicit StringConstantReference(usize const id) : m_id{ id } { }

        std::ostream& format(std::ostream& os) const override {
            return os << *this;
        }

        friend std::ostream& operator<<(std::ostream& os, StringConstantReference const& constant) {
            os << "ptr noundef @.str";
            if (constant.m_id == 0) {
                return os;
            }
            return os << '.' << constant.m_id;
        }
    };

} // namespace compiler

template<>
struct std::formatter<compiler::StringConstantReference> : OStreamFormatter { };
