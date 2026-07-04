# Full-chain tests (lexer -> parser -> interpreter)
add_executable(test_operators test_operators.cpp)
target_link_libraries(test_operators PRIVATE DemoLang)
target_compile_definitions(test_operators PRIVATE isTEST)

add_executable(test_variables test_variables.cpp)
target_link_libraries(test_variables PRIVATE DemoLang)
target_compile_definitions(test_variables PRIVATE isTEST)

add_executable(test_literals test_literals.cpp)
target_link_libraries(test_literals PRIVATE DemoLang)
target_compile_definitions(test_literals PRIVATE isTEST)

add_executable(test_controlflow test_controlflow.cpp)
target_link_libraries(test_controlflow PRIVATE DemoLang)
target_compile_definitions(test_controlflow PRIVATE isTEST)

add_executable(test_functions test_functions.cpp)
target_link_libraries(test_functions PRIVATE DemoLang)
target_compile_definitions(test_functions PRIVATE isTEST)

add_executable(test_lists test_lists.cpp)
target_link_libraries(test_lists PRIVATE DemoLang)
target_compile_definitions(test_lists PRIVATE isTEST)

add_executable(test_errors test_errors.cpp)
target_link_libraries(test_errors PRIVATE DemoLang)
target_compile_definitions(test_errors PRIVATE isTEST)

add_executable(test_integration test_integration.cpp)
target_link_libraries(test_integration PRIVATE DemoLang)
target_compile_definitions(test_integration PRIVATE isTEST)

add_executable(test_utils test_utils.cpp)
target_link_libraries(test_utils PRIVATE DemoLang)
target_compile_definitions(test_utils PRIVATE isTEST)

# Add all tests
add_test(NAME TestOperators COMMAND test_operators)
add_test(NAME TestVariables COMMAND test_variables)
add_test(NAME TestLiterals COMMAND test_literals)
add_test(NAME TestControlFlow COMMAND test_controlflow)
add_test(NAME TestFunctions COMMAND test_functions)
add_test(NAME TestLists COMMAND test_lists)
add_test(NAME TestErrors COMMAND test_errors)
add_test(NAME TestIntegration COMMAND test_integration)
add_test(NAME TestUtils COMMAND test_utils)
