project(demumble_bin LANGUAGES CXX)

add_executable(${PROJECT_NAME} main.cpp)

find_library(libdemumble NAMES demumble)

target_link_libraries(${PROJECT_NAME} PRIVATE demumble)
target_compile_definitions(${PROJECT_NAME} PRIVATE -DPROJECT_NAME="${PROJECT_NAME}")

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME demumble)

install(TARGETS	${PROJECT_NAME} DESTINATION bin)
