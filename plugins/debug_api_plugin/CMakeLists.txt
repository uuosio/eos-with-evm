file(GLOB HEADERS "include/eosio/debug_api_plugin/*.hpp")
add_library( debug_api_plugin
             debug_api_plugin.cpp
             ${HEADERS} )

target_link_libraries( debug_api_plugin http_plugin chain_plugin )
target_include_directories( debug_api_plugin PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
