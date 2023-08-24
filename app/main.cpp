#include <demumble/demumble.hpp>
#include <demumble/partial.hpp>
#include <iostream>

#include <cxxopts.hpp>

std::string getPartial(const std::string_view& sym)
{
    std::stringstream ss;
    Demumble::Partial partial{sym};
    auto res = partial.getResult();

    if (partial.isValid()) {
        ss << "Is Data:                 " << (res.isData ? "yes" : "no") << std::endl
           << "Is Ctor or Dtor:         " << (res.isCtorOrDtor ? "yes" : "no") << std::endl
           << "Is Function:             " << (res.isFunction ? "yes" : "no") << std::endl
           << "Is Special Name:         " << (res.isSpecialName ? "yes" : "no") << std::endl
           << "Has Function Qualifiers: " << (res.hasFunctionQualifiers ? "yes" : "no") << std::endl;
    } else {
        ss << "Failed to get Partial" << std::endl;
    }

    return ss.str();
}

int main(int argc, char *argv[])
{
    cxxopts::Options options("demumble", "Demangle c++ symbols");
    std::vector<std::string> symbols;
    bool partial = false;

    options.add_options()
        ("p,partial", "Partial demangle", cxxopts::value<bool>(partial))
        ("symbols", "", cxxopts::value<std::vector<std::string>>(symbols))
        ("h,help", "Print help and exit.");

    options.positional_help("<symbols...>");

    try {
        options.parse_positional({"symbols"});
        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            std::cout << options.help()
                      << std::endl;
            std::exit(0);
        }

        for (const auto& sym : symbols) {
            std::string parialStr;
            auto out = Demumble::demangle(sym);

            if (out.empty())
                out = sym;

            if (partial)
                parialStr = getPartial(sym);

            std::cout << out << std::endl;

            if (!parialStr.empty())
                std::cout << parialStr << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error parsing options: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
