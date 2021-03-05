INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_PDUZMQ pduzmq)

FIND_PATH(
    PDUZMQ_INCLUDE_DIRS
    NAMES pduzmq/api.h
    HINTS $ENV{PDUZMQ_DIR}/include
        ${PC_PDUZMQ_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    PDUZMQ_LIBRARIES
    NAMES gnuradio-pduzmq
    HINTS $ENV{PDUZMQ_DIR}/lib
        ${PC_PDUZMQ_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/pduzmqTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PDUZMQ DEFAULT_MSG PDUZMQ_LIBRARIES PDUZMQ_INCLUDE_DIRS)
MARK_AS_ADVANCED(PDUZMQ_LIBRARIES PDUZMQ_INCLUDE_DIRS)
