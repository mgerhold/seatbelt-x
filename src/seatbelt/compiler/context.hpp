#pragma once

namespace compiler {
    class FunctionDeclarationReference;
    class FunctionDeclaration;

    class Context {
    public:
        virtual ~Context() = default;

        [[nodiscard]] virtual FunctionDeclaration const& retrieve_function(FunctionDeclarationReference const& reference
        ) const = 0;
    };

} // namespace compiler
