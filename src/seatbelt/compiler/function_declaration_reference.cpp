#include "function_declaration_reference.hpp"
#include "context.hpp"

namespace compiler {

    [[nodiscard]] FunctionDeclaration const& FunctionDeclarationReference::dereference() const {
        return m_context->retrieve_function(*this);
    }

} // namespace compiler
