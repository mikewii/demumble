# demumble

`demumble` demangles both Itanium, Rust, and Visual Studio symbols. It runs on
both POSIX and Windows.

## Build instructions

    cmake -S . -B build -D CMAKE_BUILD_TYPE=Release
    cmake --build build
    sudo cmake --install build

Optionaly you can add flag `-D ENABLE_QT=ON` to link with qt core

To run tests after building:

    ctest