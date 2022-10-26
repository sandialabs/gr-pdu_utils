find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_PDU_UTILS gnuradio-pdu_utils)

FIND_PATH(
    GR_PDU_UTILS_INCLUDE_DIRS
    NAMES gnuradio/pdu_utils/api.h
    HINTS $ENV{PDU_UTILS_DIR}/include
        ${PC_PDU_UTILS_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_PDU_UTILS_LIBRARIES
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

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-pdu_utilsTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_PDU_UTILS DEFAULT_MSG GR_PDU_UTILS_LIBRARIES GR_PDU_UTILS_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_PDU_UTILS_LIBRARIES GR_PDU_UTILS_INCLUDE_DIRS)
