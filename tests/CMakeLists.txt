add_executable(test_demangle test.cpp)

target_link_libraries(test_demangle demumble)

add_test(NAME test_demangle
         COMMAND $<TARGET_FILE:test_demangle>)
