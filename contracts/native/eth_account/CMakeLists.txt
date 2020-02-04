#add_contract( utility utility utility.cpp )
add_library(eth_account eth_account.cpp
    ${CMAKE_SOURCE_DIR}/externals/eosio.contracts/contracts/eosio.token/src/eosio.token.cpp
)

target_include_directories(eth_account
   PRIVATE ${CMAKE_SOURCE_DIR}/externals/eosio.contracts/contracts/eosio.token/include
   PRIVATE "${CMAKE_SOURCE_DIR}/contracts/include"
   #                              PRIVATE "${CMAKE_SOURCE_DIR}/contracts/include/libcxx"
   #                              PRIVATE "${CMAKE_SOURCE_DIR}/contracts/include/libc"
   PRIVATE "${CMAKE_SOURCE_DIR}/contracts/include/eosiolib/core"
   PRIVATE "${CMAKE_SOURCE_DIR}/contracts/include/eosiolib/capi"
   PRIVATE "${CMAKE_SOURCE_DIR}/contracts/include/eosiolib/contracts"
)

target_compile_options( eth_account PRIVATE -DEOSIO_NATIVE -Wno-unknown-attributes )

install(TARGETS eth_account
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
