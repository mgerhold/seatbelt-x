#pragma once

namespace compiler {
    class FunctionDeclarationReference;
    class FunctionDeclaration;

    class Context {
    public:
        virtual ~Context() = default;

        // clang-format off
        [[nodiscard]] virtual FunctionDeclaration const& retrieve_function(
            FunctionDeclarationReference const& reference
        ) const = 0;
        // clang-format on
    };

} // namespace compiler
