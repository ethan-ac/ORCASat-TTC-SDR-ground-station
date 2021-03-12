INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_ZPDU zpdu)

FIND_PATH(
    ZPDU_INCLUDE_DIRS
    NAMES zpdu/api.h
    HINTS $ENV{ZPDU_DIR}/include
        ${PC_ZPDU_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    ZPDU_LIBRARIES
    NAMES gnuradio-zpdu
    HINTS $ENV{ZPDU_DIR}/lib
        ${PC_ZPDU_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/zpduTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ZPDU DEFAULT_MSG ZPDU_LIBRARIES ZPDU_INCLUDE_DIRS)
MARK_AS_ADVANCED(ZPDU_LIBRARIES ZPDU_INCLUDE_DIRS)
