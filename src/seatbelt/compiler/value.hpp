#pragma once

#include <iostream>
#include <memory>

namespace compiler {

    class BasicValue {
    public:
        virtual ~BasicValue() = default;
        virtual std::ostream& format(std::ostream& os) const = 0;
    };

    using Value = std::unique_ptr<BasicValue>;

    inline std::ostream& operator<<(std::ostream& os, Value const& value) {
        return value->format(os);
    }

} // namespace compiler
