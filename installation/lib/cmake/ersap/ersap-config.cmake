
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was ersap-config.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

####################################################################################

include(CMakeFindDependencyMacro)
find_dependency(xMsg CONFIG)

set(Ersap_CONFIG ${CMAKE_CURRENT_LIST_FILE})
find_package_handle_standard_args(Ersap CONFIG_MODE)
mark_as_advanced(Ersap_DIR)

if(NOT TARGET Ersap::ersap)
  include(${CMAKE_CURRENT_LIST_DIR}/ersap-targets.cmake)
endif()
