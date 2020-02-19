add_library(eosiolib.native STATIC 
   ${CMAKE_SOURCE_DIR}/externals/eosio.contracts/contracts/eosiolib/crypto.cpp
   ${CMAKE_SOURCE_DIR}/externals/eosio.contracts/contracts/eosiolib/eosiolib.cpp
)

target_link_libraries( eosiolib.native PUBLIC vm_api )

target_include_directories( eosiolib.native
                            PRIVATE ${CMAKE_SOURCE_DIR}/externals/eosio.contracts/contracts/eosio.token/include
                              PRIVATE "${CMAKE_SOURCE_DIR}/contracts/include"
#                              PRIVATE "${CMAKE_SOURCE_DIR}/contracts/include/libcxx"
#                              PRIVATE "${CMAKE_SOURCE_DIR}/contracts/include/libc"
                              PRIVATE "${CMAKE_SOURCE_DIR}/contracts/include/eosiolib/core"
                              PRIVATE "${CMAKE_SOURCE_DIR}/contracts/include/eosiolib/capi"
                              PRIVATE "${CMAKE_SOURCE_DIR}/contracts/include/eosiolib/contracts"
                        )
target_compile_options( eosiolib.native PRIVATE -DEOSIO_NATIVE -Wno-unknown-attributes )

install(TARGETS eosiolib.native
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)