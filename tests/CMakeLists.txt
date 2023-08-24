add_executable(demangle demangle.cpp)
add_executable(partial partial.cpp)

list(APPEND ALL_TESTS
    demangle
    partial
    )

foreach(item ${ALL_TESTS})
    target_compile_features(${item} PRIVATE cxx_std_20)

    target_link_libraries(${item} PRIVATE demumble)

    add_test(NAME ${item}
             COMMAND $<TARGET_FILE:${item}>)
endforeach()
