project(demumble CXX)
option(ENABLE_QT "Link qtcore and expose QString functions" OFF)
option(ENABLE_PREPROCESS "Preprocess symbols stripping pre/postfix" ON)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE PROJECT_HEADERS_PRIVATE CONFIGURE_DEPENDS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
set(PROJECT_FILES ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${PROJECT_HEADERS_PRIVATE})

file(GLOB_RECURSE THIRD_PARTY_SOURCES CONFIGURE_DEPENDS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/llvm/lib/Demangle/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/llvm/lib/Demangle/*.cpp")
file(GLOB_RECURSE THIRD_PARTY_HEADERS CONFIGURE_DEPENDS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/llvm/include/llvm/Demangle/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/llvm/include/llvm/Demangle/*.hpp")
set(THIRD_PARTY_FILES ${THIRD_PARTY_SOURCES} ${THIRD_PARTY_HEADERS})

add_library(llvm_demangle OBJECT ${THIRD_PARTY_FILES})
add_library(${PROJECT_NAME} $<TARGET_OBJECTS:llvm_demangle> ${PROJECT_FILES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(llvm_demangle PRIVATE third_party/llvm/include)
target_include_directories(${PROJECT_NAME} PRIVATE third_party/llvm/include)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} SYSTEM INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${PROJECT_HEADERS}")

# cpp20 for std::string_view::ends_with
target_compile_features(llvm_demangle PUBLIC cxx_std_17)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

if (ENABLE_QT)
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

    target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core)
endif()

if (UNIX)
    target_compile_options(llvm_demangle PUBLIC $<$<CONFIG:RELEASE>:-Wall>)
    target_compile_options(llvm_demangle PUBLIC $<$<CONFIG:RELEASE>:-fno-exceptions>)
    target_compile_options(llvm_demangle PUBLIC $<$<CONFIG:RELEASE>:-fno-rtti>)
    target_compile_options(llvm_demangle PUBLIC $<$<CONFIG:RELEASE>:-fvisibility=hidden>)
    target_compile_options(llvm_demangle PUBLIC $<$<CONFIG:RELEASE>:-fPIC>)
    target_compile_options(llvm_demangle PUBLIC $<$<CONFIG:RELEASE>:-fPIE>)
    target_link_options(llvm_demangle PUBLIC $<$<CONFIG:RELEASE>:-s>)

    if (${CMAKE_GENERATOR} STREQUAL "Ninja")
        target_compile_options(${PROJECT_NAME} PUBLIC $<$<CONFIG:RELEASE>:-fdiagnostics-color>)
    endif()
endif()

if (UNIX AND APPLE)
    # 10.9 chosen somewhat arbitrary; it's the first target where clang defaults
    # to libc++ and ld64 defaults to stripping __TEXT,__eh_frame.
    target_compile_options(llvm_demangle PUBLIC $<$<CONFIG:RELEASE>:-mmacosx-version-min=10.9>)
endif()

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:inline /EHs-c- /GR-")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)  # The LLVM build sets this.

    # This is apparently the simplest way to statically link the CRT in CMake:
    string(TOUPPER "${CMAKE_BUILD_TYPE}" build)

    set(flag_var "CMAKE_CXX_FLAGS_${build}")

    if(${flag_var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif()
endif()

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/pkg-config.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
    @ONLY
)

include(GNUInstallDirs)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(COMMAND create_target_directory_groups)
    create_target_directory_groups(llvm_demangle)
    create_target_directory_groups(${PROJECT_NAME})
endif()

if(ENABLE_PREPROCESS)
    add_definitions(-DENABLE_PREPROCESS)
endif()
