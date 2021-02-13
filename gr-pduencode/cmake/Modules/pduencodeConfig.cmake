INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_PDUENCODE pduencode)

FIND_PATH(
    PDUENCODE_INCLUDE_DIRS
    NAMES pduencode/api.h
    HINTS $ENV{PDUENCODE_DIR}/include
        ${PC_PDUENCODE_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    PDUENCODE_LIBRARIES
    NAMES gnuradio-pduencode
    HINTS $ENV{PDUENCODE_DIR}/lib
        ${PC_PDUENCODE_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/pduencodeTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PDUENCODE DEFAULT_MSG PDUENCODE_LIBRARIES PDUENCODE_INCLUDE_DIRS)
MARK_AS_ADVANCED(PDUENCODE_LIBRARIES PDUENCODE_INCLUDE_DIRS)
