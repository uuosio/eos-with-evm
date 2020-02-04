add_library(native_contracts SHARED 
    ${CMAKE_SOURCE_DIR}/externals/eosio.contracts/contracts/eosio.system/src/eosio.system.cpp
    ${CMAKE_SOURCE_DIR}/externals/eosio.contracts/contracts/eosio.system/include/eosio.system/eosio.system.hpp
    ${CMAKE_SOURCE_DIR}/externals/eosio.contracts/contracts/eosio.system/src/eosio.system.cpp
    ${CMAKE_SOURCE_DIR}/externals/eosio.contracts/contracts/eosio.system/src/delegate_bandwidth.cpp
    ${CMAKE_SOURCE_DIR}/externals/eosio.contracts/contracts/eosio.system/src/exchange_state.cpp
    ${CMAKE_SOURCE_DIR}/externals/eosio.contracts/contracts/eosio.system/src/native.cpp
    ${CMAKE_SOURCE_DIR}/externals/eosio.contracts/contracts/eosio.system/src/producer_pay.cpp
    ${CMAKE_SOURCE_DIR}/externals/eosio.contracts/contracts/eosio.system/src/rex.cpp
    ${CMAKE_SOURCE_DIR}/externals/eosio.contracts/contracts/eosio.system/src/voting.cpp
    ${CMAKE_SOURCE_DIR}/externals/eosio.contracts/contracts/eosio.system/src/activate_accounts.cpp

    ${CMAKE_SOURCE_DIR}/externals/eosio.contracts/contracts/eosio.token/src/eosio.token.cpp
    ${CMAKE_SOURCE_DIR}/externals/eosio.contracts/contracts/eosio.token/include/eosio.token/eosio.token.hpp
    main_eosio_token.cpp
    main_eosio_contract.cpp
)

target_link_libraries( native_contracts PRIVATE eosiolib.native vm_api )

target_include_directories( native_contracts
                            PRIVATE ${CMAKE_SOURCE_DIR}/externals/eosio.contracts/contracts/eosio.system/include
                            PRIVATE ${CMAKE_SOURCE_DIR}/externals/eosio.contracts/contracts/eosio.token/include
                            PRIVATE "${CMAKE_SOURCE_DIR}/contracts/include"
#                              PRIVATE "${CMAKE_SOURCE_DIR}/contracts/include/libcxx"
#                              PRIVATE "${CMAKE_SOURCE_DIR}/contracts/include/libc"
                              PRIVATE "${CMAKE_SOURCE_DIR}/contracts/include/eosiolib/core"
                              PRIVATE "${CMAKE_SOURCE_DIR}/contracts/include/eosiolib/capi"
                              PRIVATE "${CMAKE_SOURCE_DIR}/contracts/include/eosiolib/contracts"
                        )
target_compile_options( native_contracts PRIVATE -DEOSIO_NATIVE -Wno-unknown-attributes )

install(TARGETS native_contracts
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

copy_lib(native_contracts)
