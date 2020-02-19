add_library(test_shared SHARED main.cpp ${CMAKE_CURRENT_BINARY_DIR}/test.obj ${CMAKE_CURRENT_BINARY_DIR}/test2.obj)

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/test.obj
    COMMAND eosio-cpp -c -g -O0 -fnative -o ${CMAKE_CURRENT_BINARY_DIR}/test.obj ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating ${CMAKE_CURRENT_SOURCE_DIR}/test.obj"
)

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/test2.obj
    COMMAND eosio-cpp -c -g -O0 -fnative -o ${CMAKE_CURRENT_BINARY_DIR}/test2.obj ${CMAKE_CURRENT_SOURCE_DIR}/test2.cpp
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test2.cpp
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating ${CMAKE_CURRENT_SOURCE_DIR}/test2.obj"
)

target_link_libraries(test_shared PRIVATE vm_api)

target_include_directories(vm_api PRIVATE ${Boost_INCLUDE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/externals/magic_get/include
    PRIVATE ${CMAKE_SOURCE_DIR}/contracts
    PRIVATE ${CMAKE_BINARY_DIR}/contracts
)

# ${CMAKE_CURRENT_BINARY_DIR}/test.obj

# add_custom_target(run-tests
# COMMAND ${CMAKE_BINARY_DIR}/wabt-unittests
# COMMAND ${PYTHON_EXECUTABLE} ${RUN_TESTS_PY} --bindir ${CMAKE_BINARY_DIR}
# DEPENDS ${WABT_EXECUTABLES}
# WORKING_DIRECTORY ${WABT_SOURCE_DIR}
# ${USES_TERMINAL}
# )
