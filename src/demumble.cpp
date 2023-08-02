#include "demumble.hpp"
#include "llvm/Demangle/Demangle.h"
#include <cxxabi.h>

namespace Demumble {
static constexpr const char* postfix[] =
{
    "@Qt",
    "@GLIBC",
    "@GCC",
    "@CXXABI"
};

static std::string_view::size_type isHavePostfix(const std::string_view& mangledName)
{
    auto pos = std::string_view::npos;

    for (const auto* name : postfix) {
        pos = mangledName.rfind(name);

        if (pos != std::string_view::npos)
            return pos;
    }

    return pos;
}

std::string demangle(const char* mangledName,
                     size_t* nMangled)
{
    return Demumble::demangle(std::string_view{mangledName}, nMangled);
}

std::string demangle(const std::string& mangledName,
                     size_t* nMangled)
{
    return Demumble::demangle(static_cast<std::string_view>(mangledName)
                              , nMangled);
}

std::string demangle(const std::string_view& mangledName,
                     size_t* nMangled)
{
    std::string out;
    char* demangled = nullptr;
    const auto& sub = mangledName.substr(0, isHavePostfix(mangledName));

    do {
        ///// No itanium ISNT the same as __cxa_demangle
        demangled = __cxxabiv1::__cxa_demangle(sub.data()
                                               , nullptr
                                               , nullptr
                                               , nullptr);
        if (demangled)
            break;

        demangled = llvm::itaniumDemangle(sub);
        if (demangled)
            break;

        demangled = llvm::rustDemangle(sub);
        if (demangled)
            break;

        demangled = llvm::microsoftDemangle(sub
                                            , nMangled
                                            , nullptr);
        if (demangled)
            break;

        demangled = llvm::dlangDemangle(sub);
    } while (false);

    if (demangled) {
        out = demangled;

        free(demangled);
    } else if (mangledName != sub) {
        out = sub;
    }

    return out;
}
}; // Demumble
