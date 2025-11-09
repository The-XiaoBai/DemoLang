add_executable(tests test_main.cpp)
target_link_libraries(tests PRIVATE DemoLang)

target_compile_definitions(tests PRIVATE isTEST)

add_test(NAME tests COMMAND tests)