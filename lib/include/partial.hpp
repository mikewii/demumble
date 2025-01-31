#pragma once
#include <string>

namespace Demumble {
class PartialApi;
class Partial
{
public:
    struct Result {
        bool isData;
        bool isCtorOrDtor;
        bool isFunction;
        bool isSpecialName;
        bool hasFunctionQualifiers;

        bool operator==(const Result& other) const
        {
            return (isData == other.isData
                    && isCtorOrDtor == other.isCtorOrDtor
                    && isFunction == other.isFunction
                    && isSpecialName == other.isSpecialName
                    && hasFunctionQualifiers == other.hasFunctionQualifiers);
        }
    };
public:
    explicit Partial(const char *MangledName);
    explicit Partial(const std::string &MangledName);

    #if __cplusplus >= 201703L
    explicit Partial(const std::string_view &MangledName);
    #endif

    ~Partial();

    Result getResult(void);

    /// If this function has any any cv or reference qualifiers. These imply that
    /// the function is a non-static member function.
    bool hasFunctionQualifiers(void) const;

    /// If this symbol describes a constructor or destructor.
    bool isCtorOrDtor(void) const;

    /// If this symbol describes a function.
    bool isFunction(void) const;

    /// If this symbol describes a variable.
    bool isData(void) const;

    /// If this symbol is a <special-name>. These are generally implicitly
    /// generated by the implementation, such as vtables and typeinfo names.
    bool isSpecialName(void) const;

    bool isValid(void) const;

private:
    PartialApi* m_impl;

    bool partialDemangle(const char *MangledName);
};
}; // Demumble
