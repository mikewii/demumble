#include "partial.hpp"
#include "partialApi.hpp"
#include "tools.hpp"

#define GUARD \
if (!isValid()) \
    return false;

namespace Demumble {
Partial::Partial(const char *MangledName)
    : m_impl(nullptr)
{
#if defined(ENABLE_PREPROCESS)
    // here we might receive substring
    // need to make new string because accessing data()
    // of string_view returns whole string
    auto processed = std::string(processedSymbol(MangledName));

    (void)partialDemangle(processed.data());
#else
    (void)partialDemangle(MangledName);
#endif
}

Partial::Partial(const std::string &MangledName)
    : Partial(MangledName.data())
{
}

Partial::Partial(const std::string_view &MangledName)
    : Partial(MangledName.data())
{
}

Partial::~Partial()
{
    if (m_impl) {
        delete m_impl;
        m_impl = nullptr;
    }
}

bool Partial::partialDemangle(const char *MangledName)
{
    bool res = false;

    m_impl = new ItaniumPartial;

    if (!(res = m_impl->partialDemangle(MangledName)))
        ; // TODO: try other partial demanglers

    return res;
}

bool Partial::hasFunctionQualifiers(void) const
{
    GUARD
    return m_impl->hasFunctionQualifiers();
}

bool Partial::isCtorOrDtor(void) const
{
    GUARD
    return m_impl->isCtorOrDtor();
}

bool Partial::isFunction(void) const
{
    GUARD
    return m_impl->isFunction();
}

bool Partial::isData(void) const
{
    GUARD
    return m_impl->isData();
}

bool Partial::isSpecialName(void) const
{
    GUARD
    return m_impl->isSpecialName();
}

bool Partial::isValid(void) const
{
    return m_impl->isValid();
}
}; // Demumble
