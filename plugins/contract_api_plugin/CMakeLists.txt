file(GLOB HEADERS "include/eosio/contract_api_plugin/*.hpp")
add_library( contract_api_plugin
             contract_api_plugin.cpp
             ${HEADERS} )

target_link_libraries( contract_api_plugin http_plugin chain_plugin )
target_include_directories( contract_api_plugin PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
