#pragma once
#include "llvm/Demangle/Demangle.h"

namespace Demumble {
class PartialApi
{
public:
    PartialApi() : m_isValid(false) {}
    virtual ~PartialApi() {}

    virtual bool partialDemangle(const char *MangledName) = 0;
    virtual bool hasFunctionQualifiers(void) const = 0;

    virtual bool isCtorOrDtor(void) const = 0;
    virtual bool isFunction(void) const = 0;
    virtual bool isData(void) const = 0;
    virtual bool isSpecialName(void) const = 0;

    bool isValid(void) {return m_isValid;}

protected:
    bool m_isValid;
};

class ItaniumPartial : public PartialApi
{
public:
    ItaniumPartial();
    ~ItaniumPartial();

    bool partialDemangle(const char *MangledName) override;

    bool hasFunctionQualifiers(void) const override;

    bool isCtorOrDtor(void) const override;
    bool isFunction(void) const override;
    bool isData(void) const override;
    bool isSpecialName(void) const override;

private:
    llvm::ItaniumPartialDemangler* m_context;
};
}; // Demumble
