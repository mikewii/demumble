#include "demumble.hpp"
#include "llvm/Demangle/Demangle.h"
#include <cxxabi.h>

namespace Demumble {
namespace {
constexpr std::string_view postfix[] =
{
    "@Qt",
    "@GLIBC",
    "@GCC",
    "@CXXABI"
};

constexpr const std::pair<std::string_view, std::string_view::size_type> prefix[] =
{
    {"._", 1},
    {"_GLOBAL__sub_I_", std::string_view::npos},
};

std::string_view::size_type isHavePostfix(const std::string_view& mangledName)
{
    auto pos = std::string_view::npos;

    for (const auto& name : postfix) {
        pos = mangledName.rfind(name);

        if (pos != std::string_view::npos) {
            while (mangledName.substr(0, pos).ends_with('@')) {
                pos = mangledName.find_last_of('@') - 1;

                if (pos <= 0)
                    return std::string_view::npos;
            }

            break;
        }
    }

    return pos;
}

std::string_view::size_type isHavePrefix(const std::string_view& mangledName)
{
    for (const auto& pair : prefix) {
        const auto& str = pair.first;
        auto offset = pair.second;

        auto pos = mangledName.find(str);

        if (pos == std::string_view::npos)
            continue;

        if (offset == std::string_view::npos)
            offset = str.size();

        if (pos + offset < mangledName.size())
            return pos + offset;
    }

    return std::string_view::npos;
}
}; // anonymous

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
    std::string_view::size_type pos = std::string_view::npos;
    std::string_view sub;

    pos = isHavePrefix(mangledName);

    if (pos == std::string_view::npos)
        sub = mangledName.substr(0, isHavePostfix(mangledName));
    else
        sub = mangledName.substr(pos);

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
