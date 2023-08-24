#include "tools.hpp"

namespace Demumble {
constexpr const std::string_view postfix[] = {
    "@Qt",
    "@GLIBC",
    "@GCC",
    "@CXXABI"
};

constexpr const std::pair<std::string_view, std::string_view::size_type> prefix[] = {
    {"._", 1},
    {"_GLOBAL__sub_I_", std::string_view::npos},
};

std::string_view::size_type isHavePostfix(const std::string_view &mangledName)
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

std::string_view::size_type isHavePrefix(const std::string_view &mangledName)
{
    for (const auto& pair : prefix) {
        const auto& str = pair.first;
        auto offset = pair.second;

        auto pos = mangledName.find(str);

        if (pos == std::string_view::npos)
            continue;

        if (mangledName.substr(0, pos).find("_Z") != std::string_view::npos)
            return std::string_view::npos;

        if (offset == std::string_view::npos)
            offset = str.size();

        if (pos + offset < mangledName.size())
            return pos + offset;
    }

    return std::string_view::npos;
}

std::string_view processedSymbol(const std::string_view &mangledSymbol)
{
    std::string_view::size_type pos = std::string_view::npos;
    std::string_view out;

    pos = isHavePrefix(mangledSymbol);

    if (pos == std::string_view::npos)
        out = mangledSymbol.substr(0, isHavePostfix(mangledSymbol));
    else
        out = mangledSymbol.substr(pos);

    return out;
}
}; // Demumble
