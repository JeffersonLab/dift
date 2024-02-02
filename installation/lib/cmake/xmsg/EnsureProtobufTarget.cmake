if(NOT TARGET protobuf::libprotobuf)
  add_library(protobuf::libprotobuf UNKNOWN IMPORTED)
  set_target_properties(protobuf::libprotobuf PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${PROTOBUF_INCLUDE_DIR}")
  if(EXISTS "${PROTOBUF_LIBRARY}")
    set_target_properties(protobuf::libprotobuf PROPERTIES
      IMPORTED_LOCATION "${PROTOBUF_LIBRARY}")
  endif()
  if(UNIX AND TARGET Threads::Threads)
    set_property(TARGET protobuf::libprotobuf APPEND PROPERTY
        INTERFACE_LINK_LIBRARIES Threads::Threads)
  endif()
endif()
