#pragma once

#include "../types.hpp"

namespace compiler {
    class Context;
    class FunctionDeclaration;

    class FunctionDeclarationReference final {
        friend class FunctionCall;
        friend class Compiler;

    private:
        usize m_id;
        Context const* m_context;

    public:
        FunctionDeclarationReference(usize const id, Context const* const context)
            : m_id{ id },
              m_context{ context } { }

        [[nodiscard]] FunctionDeclaration const& dereference() const;
    };

} // namespace compiler
