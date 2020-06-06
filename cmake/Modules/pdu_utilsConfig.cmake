INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_PDU_UTILS pdu_utils)

FIND_PATH(
    PDU_UTILS_INCLUDE_DIRS
    NAMES pdu_utils/api.h
    HINTS $ENV{PDU_UTILS_DIR}/include
        ${PC_PDU_UTILS_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    PDU_UTILS_LIBRARIES
    NAMES gnuradio-pdu_utils
    HINTS $ENV{PDU_UTILS_DIR}/lib
        ${PC_PDU_UTILS_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-pdu_utilsTargets.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PDU_UTILS DEFAULT_MSG PDU_UTILS_LIBRARIES PDU_UTILS_INCLUDE_DIRS)
MARK_AS_ADVANCED(PDU_UTILS_LIBRARIES PDU_UTILS_INCLUDE_DIRS)
