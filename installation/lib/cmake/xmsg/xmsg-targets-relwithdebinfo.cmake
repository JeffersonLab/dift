#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "xMsg::xmsg" for configuration "RelWithDebInfo"
set_property(TARGET xMsg::xmsg APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(xMsg::xmsg PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libxmsg.so"
  IMPORTED_SONAME_RELWITHDEBINFO "libxmsg.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS xMsg::xmsg )
list(APPEND _IMPORT_CHECK_FILES_FOR_xMsg::xmsg "${_IMPORT_PREFIX}/lib/libxmsg.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
