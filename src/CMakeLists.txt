file(GLOB SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_library(DemoLang STATIC ${SOURCES})

target_include_directories(DemoLang PRIVATE ${CMAKE_SOURCE_DIR}/include)

set_target_properties(DemoLang PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

if(MSVC)
  set_target_properties(DemoLang PROPERTIES
    LINK_FLAGS "/INCREMENTAL:NO"
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )

  target_compile_options(DemoLang PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

add_executable(main main.cpp)
target_link_libraries(main PRIVATE DemoLang)