configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/eosio/chain/core_symbol.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/include/eosio/chain/core_symbol.hpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/genesis_state_root_key.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/genesis_state_root_key.cpp)

file(GLOB HEADERS "include/eosio/chain/*.hpp"
                  "include/eosio/chain/webassembly/*.hpp"
                  "${CMAKE_CURRENT_BINARY_DIR}/include/eosio/chain/core_symbol.hpp" )

## SORT .cpp by most likely to change / break compile
add_library( eosio_chain
             merkle.cpp
             name.cpp
             transaction.cpp
             block_header.cpp
             block_header_state.cpp
             block_state.cpp
             fork_database.cpp
             controller.cpp
             authorization_manager.cpp
             resource_limits.cpp
             block_log.cpp
             transaction_context.cpp
             eosio_contract.cpp
             eosio_contract_abi.cpp
             chain_config.cpp
             chain_id_type.cpp
             genesis_state.cpp
             ${CMAKE_CURRENT_BINARY_DIR}/genesis_state_root_key.cpp

#             chain_config.cpp
#             block_trace.cpp
              wast_to_wasm.cpp
              wasm_interface.cpp
              wasm_eosio_validation.cpp
              wasm_eosio_injection.cpp
              apply_context.cpp
              abi_serializer.cpp
              asset.cpp
              snapshot.cpp

             webassembly/wavm.cpp
             webassembly/wabt.cpp

#             get_config.cpp
#
#             contracts/chain_initializer.cpp

             trace.cpp
             transaction_metadata.cpp
             vm_api/vm_api.cpp
             "${CMAKE_SOURCE_DIR}/contracts/eosio.token/eosio.token.cpp"
             protocol_state_object.cpp
             protocol_feature_activation.cpp
             protocol_feature_manager.cpp
             genesis_intrinsics.cpp
             whitelisted_intrinsics.cpp
             thread_utils.cpp
             ${HEADERS}
             )

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
else()
      set(MOVEMV_LIB "${CMAKE_SOURCE_DIR}/externals/libra/target/release/libvm_move${CMAKE_SHARED_LIBRARY_SUFFIX}")
endif ()

target_link_libraries( eosio_chain fc chainbase Logging IR WAST WASM Runtime
                       softfloat builtins wabt eosiolib_native ${MOVEMV_LIB} vm_eos
                     )
target_include_directories( eosio_chain
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/../wasm-jit/Include"
                                   "${CMAKE_SOURCE_DIR}/libraries/wabt"
                                   "${CMAKE_BINARY_DIR}/libraries/wabt"
                              PRIVATE "${CMAKE_SOURCE_DIR}/contracts/eosiolib_native/include"
                              PRIVATE "${CMAKE_SOURCE_DIR}/contracts"
                              PRIVATE "${CMAKE_BINARY_DIR}/contracts"
                              PRIVATE ${CMAKE_SOURCE_DIR}/externals/magic_get/include
                        )

add_custom_target(movevm
      COMMAND cargo build -p vm_move --release
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/externals/libra
      DEPENDS ${CMAKE_SOURCE_DIR}/externals/libra/language/vm/vm_runtime/vm_move/build.rs
)

add_dependencies(movevm eosiolib_native)
add_dependencies(eosio_chain movevm)

install( TARGETS eosio_chain
   RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR}
   LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
   ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
)
install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/eosio/chain/
      DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/eosio/chain
      FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h" PATTERN "webassembly" EXCLUDE
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/eosio/chain/core_symbol.hpp DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/eosio/chain)
#if(MSVC)
#  set_source_files_properties( db_init.cpp db_block.cpp database.cpp block_log.cpp PROPERTIES COMPILE_FLAGS "/bigobj" )
#endif(MSVC)
