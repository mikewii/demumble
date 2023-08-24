#pragma once
#include <string_view>

namespace Demumble {
extern std::string_view::size_type isHavePostfix(const std::string_view& mangledName);
extern std::string_view::size_type isHavePrefix(const std::string_view& mangledName);
extern std::string_view processedSymbol(const std::string_view& mangledSymbol);
}
