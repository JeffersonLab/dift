#.rst:
# FindZeroMQ
# ----------
#
# Find the native ZeroMQ headers and library.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module will set the following variables in your project:
#
# ``ZeroMQ_INCLUDE_DIRS``
#   where to find zmq.h, etc.
# ``ZeroMQ_LIBRARIES``
#   the libraries to link against to use ZeroMQ.
# ``ZeroMQ_FOUND``
#   true if the ZeroMQ headers and libraries were found.

find_package(PkgConfig QUIET)

pkg_check_modules(PC_LIBZMQ QUIET libzmq)
# https://gitlab.kitware.com/cmake/cmake/commit/f5c46dd84ec48cfd1bfe32ed0093a21321e89845
mark_as_advanced(pkgcfg_lib_PC_LIBZMQ_zmq)

# Look for the header file.
find_path(ZeroMQ_INCLUDE_DIR NAMES zmq.h HINTS ${PC_LIBZMQ_INCLUDE_DIRS})

# Look for the library.
find_library(ZeroMQ_LIBRARY NAMES zmq libzmq HINTS ${PC_LIBZMQ_LIBRARY_DIRS})

if(ZeroMQ_INCLUDE_DIR AND EXISTS "${ZeroMQ_INCLUDE_DIR}/zmq.h")
  file(READ "${ZeroMQ_INCLUDE_DIR}/zmq.h" _ZMQ_VERSION_H_CONTENTS)
  string(REGEX MATCH "#define ZMQ_VERSION_MAJOR ([0-9])" _MATCH "${_ZMQ_VERSION_H_CONTENTS}")
  set(ZeroMQ_VERSION_MAJOR ${CMAKE_MATCH_1})
  string(REGEX MATCH "#define ZMQ_VERSION_MINOR ([0-9])" _MATCH "${_ZMQ_VERSION_H_CONTENTS}")
  set(ZeroMQ_VERSION_MINOR ${CMAKE_MATCH_1})
  string(REGEX MATCH "#define ZMQ_VERSION_PATCH ([0-9])" _MATCH "${_ZMQ_VERSION_H_CONTENTS}")
  set(ZeroMQ_VERSION_PATCH ${CMAKE_MATCH_1})
  set(ZeroMQ_VERSION "${ZeroMQ_VERSION_MAJOR}.${ZeroMQ_VERSION_MINOR}.${ZeroMQ_VERSION_PATCH}")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ZeroMQ
  REQUIRED_VARS ZeroMQ_LIBRARY ZeroMQ_INCLUDE_DIR
  VERSION_VAR ZeroMQ_VERSION
)

if(ZeroMQ_FOUND)
  set(ZeroMQ_LIBRARIES ${ZeroMQ_LIBRARY})
  set(ZeroMQ_INCLUDE_DIRS ${ZeroMQ_INCLUDE_DIR})

  if(NOT TARGET ZeroMQ::libzmq)
    add_library(ZeroMQ::libzmq SHARED IMPORTED)
    set_target_properties(ZeroMQ::libzmq PROPERTIES
        IMPORTED_LOCATION ${ZeroMQ_LIBRARY}
        INTERFACE_INCLUDE_DIRECTORIES ${ZeroMQ_INCLUDE_DIR}
    )
  endif()
endif()

mark_as_advanced(ZeroMQ_LIBRARY ZeroMQ_INCLUDE_DIR)
