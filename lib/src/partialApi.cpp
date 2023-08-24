#include "partialApi.hpp"

namespace Demumble {
ItaniumPartial::ItaniumPartial()
    : m_context(new llvm::ItaniumPartialDemangler)
{
}

ItaniumPartial::~ItaniumPartial()
{
    delete m_context;
}

bool ItaniumPartial::partialDemangle(const char *MangledName)
{
    m_isValid = !m_context->partialDemangle(MangledName);

    return m_isValid;
}

bool ItaniumPartial::hasFunctionQualifiers(void) const
{
    return m_context->hasFunctionQualifiers();
}

bool ItaniumPartial::isCtorOrDtor(void) const
{
    return m_context->isCtorOrDtor();
}

bool ItaniumPartial::isFunction(void) const
{
    return m_context->isFunction();
}

bool ItaniumPartial::isData(void) const
{
    return m_context->isData();
}

bool ItaniumPartial::isSpecialName(void) const
{
    return m_context->isSpecialName();
}
}; // Demumble
