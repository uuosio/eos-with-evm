if( EOSIO_COMPILE_EOSIO_CONTRACTS )
   include(ExternalProject)

   set(EOSIO_WASM_OLD_BEHAVIOR "Off")
   find_package(eosio.cdt REQUIRED)

   message( STATUS "Building contracts in directory `eosio.bios`" )
   ExternalProject_Add(
      bios_contract_project
      SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/eosio.bios
      BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/eosio.bios
      CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE=${EOSIO_CDT_ROOT}/lib/cmake/eosio.cdt/EosioWasmToolchain.cmake -DEOSIO_COMPILE_TEST_CONTRACTS=${EOSIO_COMPILE_TEST_CONTRACTS}
      UPDATE_COMMAND ""
      PATCH_COMMAND ""
      TEST_COMMAND ""
      INSTALL_COMMAND ""
      BUILD_ALWAYS 1
   )
endif()

add_subdirectory( eosiolib )
add_subdirectory( vm_api )
add_subdirectory( native )
add_subdirectory( test )
