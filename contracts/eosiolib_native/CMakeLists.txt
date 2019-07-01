configure_file(${CMAKE_CURRENT_SOURCE_DIR}/build.rs.in ${CMAKE_SOURCE_DIR}/externals/libra/language/vm/vm_runtime/vm_move/build.rs)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/build.rs.in ${CMAKE_SOURCE_DIR}/externals/libra/language/vm/vm_runtime/build.rs)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/build.rs.in ${CMAKE_SOURCE_DIR}/externals/libra/language/stdlib/natives/build.rs)

add_library(eosiolib_native SHARED 
eosiolib.cpp
action.cpp
chain.cpp
crypto.cpp
db.cpp
memory.cpp
permission.cpp
print.cpp
privileged.cpp
system.cpp
transaction.cpp
)

target_link_libraries( eosiolib_native PUBLIC softfloat)

target_include_directories(eosiolib_native PRIVATE ${Boost_INCLUDE_DIR}
#   PRIVATE ${CMAKE_SOURCE_DIR}/contracts/libc++/upstream/include
#   PRIVATE ${CMAKE_SOURCE_DIR}/contracts/libc++/include/libc++/upstream/include
#   PRIVATE ${CMAKE_SOURCE_DIR}/contracts/libc++/musl/include/musl/upstream/include
#    PRIVATE ${CMAKE_SOURCE_DIR}/contracts/musl/upstream/include
    PRIVATE ${CMAKE_SOURCE_DIR}/externals/magic_get/include
    PRIVATE ${CMAKE_SOURCE_DIR}/contracts
    PUBLIC ${CMAKE_SOURCE_DIR}/libraries/chain/include
    PUBLIC ${CMAKE_SOURCE_DIR}/libraries/fc/include
    PUBLIC ${CMAKE_SOURCE_DIR}/libraries/softfloat/source/include
    )
