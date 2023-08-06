#include <demumble/demumble.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc <= 1)
        std::cout << "Usage: " << PROJECT_NAME << " [symbol...]" << std::endl;

    for (auto i = 1; i < argc; ++i) {
        auto out = Demumble::demangle(argv[i]);

        if (out.empty())
            out = argv[i];

        std::cout << out << std::endl;
    }

    return 0;
}
