#include "../include/demumble.hpp"
#include <cassert>
#include <map>

void demangleGeneral(void)
{
    const std::map<std::string, std::string> testStrings =
    {
#ifdef __APPLE__
        {"_Znwi", "operator new(int)"},
#else
        {"__Znwi", "operator new(int)"},
#endif
        {"_Z4funcPci", "func(char*, int)"},
        {"_Z1fv", "f()"},
        {"_RINvNtC3std3mem8align_ofdE", "std::mem::align_of::<f64>"},
        {"_RNvNvC5mylib3foo3bar", "mylib::foo::bar"},
        {"?Fxi@@YAHP6AHH@Z@Z", "int __cdecl Fxi(int (__cdecl *)(int))"},
        {"??0S@@QEAA@$$QEAU0@@Z", "public: __cdecl S::S(struct S &&)"},
        {"??_C@_02PCEFGMJL@hi?$AA@", "\"hi\""},
        {"___Z10blocksNRVOv_block_invoke", "invocation function for block in blocksNRVO()"},
        {"_ZN2zx7channelD4Ev", "zx::channel::~channel()"},
        {"_ZZ3fooiENK3$_0clEi", "foo(int)::$_0::operator()(int) const"},
        {".?AVNet@@", "class Net `RTTI Type Descriptor Name'"},
        {"_ZN16InLoadingGameBoxC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKN10Filesystem4PathE8LoadTypeb", "InLoadingGameBox::InLoadingGameBox(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, Filesystem::Path const&, LoadType, bool)"},
        {"_ZN7QObject7connectIM9QComboBoxFviEZN15ParseParametersC4EP7QWidgetEUliE4_EENSt9enable_ifIXeqsrN9QtPrivate15FunctionPointerIT0_EE13ArgumentCountLin1EEN11QMetaObject10ConnectionEE4typeEPKNSA_IT_E6ObjectESH_PKS_SB_N2Qt14ConnectionTypeE", "std::enable_if<QtPrivate::FunctionPointer<ParseParameters::ParseParameters(QWidget*)::{lambda(int)#6}>::ArgumentCount==(-1), QMetaObject::Connection>::type QObject::connect<void (QComboBox::*)(int), ParseParameters::ParseParameters(QWidget*)::{lambda(int)#6}>(QtPrivate::FunctionPointer<void (QComboBox::*)(int)>::Object const*, void (QComboBox::*)(int), QObject const*, ParseParameters::ParseParameters(QWidget*)::{lambda(int)#6}, Qt::ConnectionType)"}
    };

    for (const auto& pair : testStrings) {
        const auto& mangled = pair.first;
        const auto& demangled = pair.second;
        const auto& result = Demumble::demangle(mangled);

        if (demangled != result) {
            fprintf(stderr
                    , "%s\n!=\n%s\n"
                    , demangled.c_str()
                    , result.c_str());

            assert(demangled == result);
        }
    }
}

void demangleQt(void)
{
    const std::map<std::string, std::string> testStrings =
        {
            {"_ZN7QWidget11setGeometryERK5QRect@Qt_5", "QWidget::setGeometry(QRect const&)"}
        };

    for (const auto& pair : testStrings) {
        const auto& mangled = pair.first;
        const auto& demangled = pair.second;
        const auto& result = Demumble::demangle(mangled);

        if (demangled != result) {
            fprintf(stderr
                    , "%s\n!=\n%s\n"
                    , demangled.c_str()
                    , result.c_str());

            assert(demangled == result);
        }
    }
}

int main(void)
{
    demangleGeneral();
    demangleQt();

    return 0;
}
