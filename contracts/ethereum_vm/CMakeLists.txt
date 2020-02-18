if (EOSIO_COMPILE_ETHEREUM_VM_CONTRACTS)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package( eosio.cdt REQUIRED )

add_contract(ethereum_vm ethereum_vm 
    ${CMAKE_SOURCE_DIR}/../../externals/aleth/evm4eos/ethereum_vm.cpp
)

target_include_directories(ethereum_vm
   PUBLIC
   ${CMAKE_SOURCE_DIR}/../../externals/aleth/evm4eos
)

target_compile_definitions(ethereum_vm PRIVATE USE_INTRINSIC_EVM_RECOVER_KEY USE_INTRINSIC_EVM_GET_ACCOUNT_ID USE_INTRINSIC_EVM_EXECUTE)

set_target_properties(ethereum_vm
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

configure_file( ${CMAKE_SOURCE_DIR}/../../externals/aleth/evm4eos/ethereum_vm.abi ${CMAKE_CURRENT_BINARY_DIR}/ethereum_vm.abi @COPYONLY )

else()

#add_contract( utility utility utility.cpp )
add_library(ethereum_vm ${CMAKE_SOURCE_DIR}/externals/aleth/evm4eos/eth_account.cpp
)

target_include_directories(ethereum_vm
   PRIVATE ${CMAKE_SOURCE_DIR}/externals/aleth/evm4eos
   PRIVATE ${CMAKE_SOURCE_DIR}/externals/eosio.contracts/contracts/eosio.token/include
   PRIVATE "${CMAKE_SOURCE_DIR}/contracts/include"
   #                              PRIVATE "${CMAKE_SOURCE_DIR}/contracts/include/libcxx"
   #                              PRIVATE "${CMAKE_SOURCE_DIR}/contracts/include/libc"
   PRIVATE "${CMAKE_SOURCE_DIR}/contracts/include/eosiolib/core"
   PRIVATE "${CMAKE_SOURCE_DIR}/contracts/include/eosiolib/capi"
   PRIVATE "${CMAKE_SOURCE_DIR}/contracts/include/eosiolib/contracts"
)

target_compile_options( ethereum_vm PRIVATE -DEOSIO_NATIVE -Wno-unknown-attributes )

install(TARGETS ethereum_vm
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

endif()


