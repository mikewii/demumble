#include "../include/demumble.hpp"
#include <cassert>
#include <map>

void testDemangle(void)
{
    const std::map<std::string, std::string> testStrings =
    {
#ifdef __APPLE__
        {"_Znwi", "operator new(int)"},
#else
        {"__Znwi", "operator new(int)"},
#endif
        {"hello", "hello"},
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
        {".?AVNet@@", "class Net `RTTI Type Descriptor Name'"}
    };

    for (const auto& pair : testStrings) {
        const auto& mangled = pair.first;
        const auto& demangled = pair.second;
        const auto& result = Demumble::demangle(mangled);

//        fprintf(stderr, "[%s] [%s]\n"
//                , demangled.c_str()
//                , result.c_str());

        assert(demangled == result);
    }
}

int main(void)
{
    testDemangle();

    return 0;
}
