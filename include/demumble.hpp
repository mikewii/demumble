#pragma once
#include <string>

#if __cplusplus >= 201703L
#include <string_view>
#endif

#ifdef QT_CORE_LIB
#include <QString>
#endif

namespace Demumble {
std::string demangle(const char* mangledName,
                     size_t* nMangled = nullptr);
std::string demangle(const std::string& mangledName,
                     size_t* nMangled = nullptr);

#if __cplusplus >= 201703L
std::string demangle(const std::string_view& mangledName,
                     size_t* nMangled = nullptr);
#endif

#ifdef QT_CORE_LIB
QString demangle(const QString& mangledName,
                 size_t* nMangled = nullptr);
#endif
}; // Demumble
