add_library(vm_api SHARED 
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
#utility.cpp
vm_api.cpp
)

if (APPLE)
    set(_symbols_list "${CMAKE_CURRENT_SOURCE_DIR}/symbols.list")
    set(LINK_FLAGS "${LINK_FLAGS} -Wl,-exported_symbols_list,'${_symbols_list}'")
else()
    set(_version_script "${CMAKE_CURRENT_SOURCE_DIR}/version.script")
    set(LINK_FLAGS "${LINK_FLAGS} -Wl,--version-script,\"${_version_script}\"")
endif (APPLE)

target_link_libraries( vm_api PUBLIC softfloat)
set_target_properties(vm_api  PROPERTIES LINK_FLAGS "${LINK_FLAGS}")

target_include_directories(vm_api PRIVATE ${Boost_INCLUDE_DIR}
#   PRIVATE ${CMAKE_SOURCE_DIR}/contracts/libc++/upstream/include
#   PRIVATE ${CMAKE_SOURCE_DIR}/contracts/libc++/include/libc++/upstream/include
#   PRIVATE ${CMAKE_SOURCE_DIR}/contracts/libc++/musl/include/musl/upstream/include
#    PRIVATE ${CMAKE_SOURCE_DIR}/contracts/musl/upstream/include
    PRIVATE ${CMAKE_SOURCE_DIR}/externals/magic_get/include
    PRIVATE ${CMAKE_SOURCE_DIR}/contracts
    PRIVATE ${CMAKE_BINARY_DIR}/contracts
    PUBLIC ${CMAKE_SOURCE_DIR}/libraries/fc/include
    PUBLIC ${CMAKE_SOURCE_DIR}/libraries/softfloat/source/include
)


# add_library(vm_api SHARED 
#     vm_api.cpp
# )
    
# #add_dependencies(vm_api native_project)

# target_link_libraries( vm_api PUBLIC softfloat)

# target_include_directories(vm_api PRIVATE ${Boost_INCLUDE_DIR}
# #   PRIVATE ${CMAKE_SOURCE_DIR}/contracts/libc++/upstream/include
# #   PRIVATE ${CMAKE_SOURCE_DIR}/contracts/libc++/include/libc++/upstream/include
# #   PRIVATE ${CMAKE_SOURCE_DIR}/contracts/libc++/musl/include/musl/upstream/include
# #    PRIVATE ${CMAKE_SOURCE_DIR}/contracts/musl/upstream/include
#    PRIVATE ${CMAKE_SOURCE_DIR}/externals/magic_get/include
#    PRIVATE ${CMAKE_SOURCE_DIR}/contracts
#    PRIVATE ${CMAKE_BINARY_DIR}/contracts
#    PUBLIC ${CMAKE_SOURCE_DIR}/libraries/chain/include
#    PUBLIC ${CMAKE_SOURCE_DIR}/libraries/fc/include
#    PUBLIC ${CMAKE_SOURCE_DIR}/libraries/softfloat/source/include
# )

install(TARGETS vm_api
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

copy_lib(vm_api)
