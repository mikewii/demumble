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
        if ((demangled = __cxxabiv1::__cxa_demangle(sub.data()
                                                    , nullptr
                                                    , nullptr
                                                    , nullptr))
            || (demangled = llvm::itaniumDemangle(sub))
            || (demangled = llvm::rustDemangle(sub))
            || (demangled = llvm::microsoftDemangle(sub
                                                    , nMangled
                                                    , nullptr
                                                    , llvm::MSDemangleFlags::MSDF_None))
            || (demangled = llvm::dlangDemangle(sub)))
            break;
    } while (false);

    if (demangled) {
        out = demangled;

        std::free(demangled);
    } else if (mangledName != sub) {
        out = sub;
    }

    return out;
}
}; // Demumble
