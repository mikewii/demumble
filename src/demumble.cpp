#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "llvm/Demangle/Demangle.h"
#include "demumble.hpp"

namespace Demumble {
std::string demangle(const char* mangledName,
                     size_t* nMangled)
{
    std::string out;
    char* demangled = nullptr;

    do {
        demangled = llvm::itaniumDemangle({mangledName});
        if (demangled)
            break;

        demangled = llvm::rustDemangle({mangledName});
        if (demangled)
            break;

        demangled = llvm::microsoftDemangle({mangledName}, nMangled, nullptr);
        break;
    } while (true);

    if (demangled) {
        out = demangled;

        free(demangled);
    } else {
        out = mangledName;
    }

    return out;
}

std::string demangle(const std::string &mangledName,
                     size_t *nMangled)
{
    return demangle(mangledName.c_str(), nMangled);
}

#if __cplusplus >= 201703L
std::string demangle(const std::string_view &mangledName,
                     size_t *nMangled)
{
    std::string out;
    char* demangled = nullptr;

    do {
        demangled = llvm::itaniumDemangle(mangledName);
        if (demangled)
            break;

        demangled = llvm::rustDemangle(mangledName);
        if (demangled)
            break;

        demangled = llvm::microsoftDemangle(mangledName, nMangled, nullptr);
        break;
    } while (true);

    if (demangled) {
        out = demangled;

        free(demangled);
    } else {
        out = mangledName;
    }

    return out;
}
#endif

#ifdef QT_CORE_LIB
std::string demangle(const QString& mangledName,
                     size_t* nMangled)
{
    return demangle(mangledName.toStdString(), nMangled);
}
#endif

}; // Demumble
