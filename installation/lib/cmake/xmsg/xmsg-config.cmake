
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was xmsg-config.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

####################################################################################

set(OLD_CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

include(CMakeFindDependencyMacro)
find_dependency(ZeroMQ 4.0)
find_dependency(Protobuf 2.5)
include(EnsureProtobufTarget)

set(CMAKE_MODULE_PATH ${OLD_CMAKE_MODULE_PATH})
unset(OLD_CMAKE_MODULE_PATH)

set(xMsg_CONFIG ${CMAKE_CURRENT_LIST_FILE})
find_package_handle_standard_args(xMsg CONFIG_MODE)
mark_as_advanced(xMsg_DIR)

if(NOT TARGET xMsg::xmsg)
  include(${CMAKE_CURRENT_LIST_DIR}/xmsg-targets.cmake)
endif()
