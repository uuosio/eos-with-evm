macro( copy_bin file )
   add_custom_command( TARGET ${file} POST_BUILD COMMAND mkdir -p ${CMAKE_BINARY_DIR}/bin )
   add_custom_command( TARGET ${file} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${file}${CMAKE_EXECUTABLE_SUFFIX} ${CMAKE_BINARY_DIR}/bin/ )
endmacro( copy_bin )

macro( copy_lib file )
   add_custom_command( TARGET ${file} POST_BUILD COMMAND mkdir -p ${CMAKE_BINARY_DIR}/lib )
   add_custom_command( TARGET ${file} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:${file}> ${CMAKE_BINARY_DIR}/lib/ )
endmacro( copy_lib )
