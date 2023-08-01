#include "demumble.hpp"
#include "llvm/Demangle/Demangle.h"
#include <cxxabi.h>

namespace Demumble {
static std::string::size_type isQt(const std::string& mangledName)
{
    return mangledName.rfind("@Qt");
}

#if __cplusplus >= 201703L
static std::string_view::size_type isQt(const std::string_view& mangledName)
{
    return mangledName.rfind("@Qt");
}
#endif

std::string demangle(const char* mangledName,
                     size_t* nMangled)
{
    return Demumble::demangle(std::string{mangledName}, nMangled);
}

std::string demangle(const std::string& mangledName,
                     size_t* nMangled)
{
    std::string out;
    char* demangled = nullptr;

    do {
        ///// No itanium ISNT the same as __cxa_demangle
        demangled = __cxxabiv1::__cxa_demangle(mangledName.substr(0, isQt(mangledName)).data()
                                               , nullptr
                                               , nullptr
                                               , nullptr);
        if (demangled)
            break;

        demangled = llvm::itaniumDemangle(mangledName.substr(0, isQt(mangledName)));
        if (demangled)
            break;

        demangled = llvm::rustDemangle(mangledName.substr(0, isQt(mangledName)));
        if (demangled)
            break;

        demangled = llvm::microsoftDemangle(mangledName.substr(0, isQt(mangledName))
                                            , nMangled
                                            , nullptr);
        if (demangled)
            break;

        demangled = llvm::dlangDemangle(mangledName.substr(0, isQt(mangledName)));
    } while (false);

    if (demangled) {
        out = demangled;

        free(demangled);
    }

    return out;
}

#if __cplusplus >= 201703L
std::string demangle(const std::string_view& mangledName,
                     size_t* nMangled)
{
    std::string out;
    char* demangled = nullptr;

    do {
        demangled = __cxxabiv1::__cxa_demangle(mangledName.substr(0, isQt(mangledName)).data()
                                               , nullptr
                                               , nullptr
                                               , nullptr);
        if (demangled)
            break;

        demangled = llvm::itaniumDemangle(mangledName.substr(0, isQt(mangledName)));
        if (demangled)
            break;

        demangled = llvm::rustDemangle(mangledName.substr(0, isQt(mangledName)));
        if (demangled)
            break;

        demangled = llvm::microsoftDemangle(mangledName.substr(0, isQt(mangledName))
                                            , nMangled
                                            , nullptr);
        if (demangled)
            break;

        demangled = llvm::dlangDemangle(mangledName.substr(0, isQt(mangledName)));
    } while (false);

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
