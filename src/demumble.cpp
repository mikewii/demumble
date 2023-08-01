#include "demumble.hpp"
#include "llvm/Demangle/Demangle.h"

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
        if (demangled)
            break;

        demangled = llvm::dlangDemangle({mangledName});
        break;
    } while (true);

    if (demangled) {
        out = demangled;

        free(demangled);
    }

    return out;
}

std::string demangle(const std::string& mangledName,
                     size_t* nMangled)
{
    return Demumble::demangle(mangledName.c_str()
                              , nMangled);
}

#if __cplusplus >= 201703L
std::string demangle(const std::string_view& mangledName,
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
        if (demangled)
            break;

        demangled = llvm::dlangDemangle({mangledName});
        break;
    } while (true);

    if (demangled) {
        out = demangled;

        free(demangled);
    }

    return out;
}
#endif

#ifdef QT_CORE_LIB
QString demangle(const QString& mangledName,
                 size_t* nMangled)
{
    return QString::fromStdString(Demumble::demangle(mangledName.toStdString()
                                                     , nMangled));
}
#endif
}; // Demumble
