add_executable(test_lexer test_lexer.cpp)
target_link_libraries(test_lexer PRIVATE DemoLang)
target_compile_definitions(test_lexer PRIVATE isTEST)

add_executable(test_parser test_parser.cpp)
target_link_libraries(test_parser PRIVATE DemoLang)
target_compile_definitions(test_parser PRIVATE isTEST)

add_executable(test_interpreter test_interpreter.cpp)
target_link_libraries(test_interpreter PRIVATE DemoLang)
target_compile_definitions(test_interpreter PRIVATE isTEST)

add_executable(test_utils test_utils.cpp)
target_link_libraries(test_utils PRIVATE DemoLang)
target_compile_definitions(test_utils PRIVATE isTEST)

add_test(NAME TestLexer COMMAND test_lexer)
add_test(NAME TestParser COMMAND test_parser)
add_test(NAME TestInterpreter COMMAND test_interpreter)
add_test(NAME TestUtils COMMAND test_utils)