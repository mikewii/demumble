#include "../include/partial.hpp"
#include "../include/demumble.hpp"

#include <cassert>
#include <cstdio>
#include <array>
#include <string_view>
#include <tuple>

namespace {
struct partial {
    bool isData;
    bool isCtorOrDtor;
    bool isFunction;
    bool isSpecialName;
    bool hasFunctionQualifiers;

    bool operator==(const partial& other) const
    {
        return std::tie(isData
                        , isCtorOrDtor
                        , isFunction
                        , isSpecialName
                        , hasFunctionQualifiers)
               ==
               std::tie(other.isData
                        , other.isCtorOrDtor
                        , other.isFunction
                        , other.isSpecialName
                        , other.hasFunctionQualifiers);
    }
};

using string_type = std::string_view;
using items = std::tuple<string_type, partial>;

constexpr const items data[] = {
    {"_ZZN10RenderUtil30drawLineRotatedEntityInfoIconsER9DrawQueueRK11MapPosition15RealOrientationffPKPK13ItemPrototypemNS_9IconLayerEbE2dx"
     , {1, 0, 0, 0, 0}},
    {"_ZN13SpriteOptions10TreeNormalE"
     , {1, 0, 0, 0, 0}},
};

constexpr const items ctorDtor[] = {
    {"_ZN2zx7channelD4Ev"
     , {0, 1, 1, 0, 0}},
    {"_ZN16InLoadingGameBoxC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKN10Filesystem4PathE8LoadTypeb"
     , {0, 1, 1, 0, 0}},
    {"_ZN22ProgrammableSpeakerGuiC1EPK19ProgrammableSpeakerR16GuiActionHandlerR15GuiInfoProvider"
     , {0, 1, 1, 0, 0}},
};

constexpr const items function[] = {
    {"_Znwi"
     , {0, 0, 1, 0, 0}},
    {"__Znwi"
     , {0, 0, 1, 0, 0}},
    {"_Z4funcPci"
     , {0, 0, 1, 0, 0}},
    {"_Z1fv"
     , {0, 0, 1, 0, 0}},
    {"_ZN6Entity17finishEntityCloneEPKS_PS_RSt3mapIS1_S2_St4lessIS1_ESaISt4pairIKS1_S2_EEE"
     , {0, 0, 1, 0, 0}},
};

constexpr const items functionQualifiers[] = {
    {"_ZZ3fooiENK3$_0clEi"
     , {0, 0, 1, 0, 1}},
    {"_ZNK11BoundingBox4saveER12PropertyTreeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE"
     , {0, 0, 1, 0, 1}},
};

constexpr const items specialName[] = {
    {"___Z10blocksNRVOv_block_invoke"
     , {0, 0, 0, 1, 0}},
    {"_ZTS19UnitSpawnDefinition"
     , {0, 0, 0, 1, 0}},
};

template <std::size_t N>
void partial(const items (&items)[N])
{
    for (const auto& item : items) {
        const auto& preparedMangled = std::get<0>(item);
        const auto& preparedPartial = std::get<1>(item);

        const auto& resultPartial = Demumble::Partial(preparedMangled);
        const auto& redultDemangled = Demumble::demangle(preparedMangled);
        struct partial resultPartialStruct{resultPartial.isData()
                                           , resultPartial.isCtorOrDtor()
                                           , resultPartial.isFunction()
                                           , resultPartial.isSpecialName()
                                           , resultPartial.hasFunctionQualifiers()};

        fprintf(stderr,
                "SYM: %s\n" \
                "partial isValid: %d\n" \
                "isData: %d\n" \
                "isCtorDtor: %d\n" \
                "isFunction: %d\n" \
                "isSpecialName: %d\n" \
                "hasFunctionQualifiers: %d\n\n"
                , redultDemangled.data()
                , resultPartial.isValid()
                , resultPartialStruct.isData
                , resultPartialStruct.isCtorOrDtor
                , resultPartialStruct.isFunction
                , resultPartialStruct.isSpecialName
                , resultPartialStruct.hasFunctionQualifiers);

        assert(preparedPartial == resultPartialStruct);
    }
}
}; //anonymous

int main(void)
{
    partial(data);
    partial(ctorDtor);
    partial(function);
    partial(specialName);
    partial(functionQualifiers);

    return 0;
}
