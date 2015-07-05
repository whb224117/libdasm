# Locate DASM library
# This module defines
# DASM_LIBRARY, the name of the library to link against
# DASM_INCLUDE_DIR, where to find libdasm.h
#

find_path(DASM_INCLUDE_DIR libdasm.h
    HINTS
    $ENV{DASMDIR}
    PATH_SUFFIXES include
    PATHS
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local/include/
    /usr/include/
    /sw # Fink
    /opt/local # DarwinPorts
    /opt/csw # Blastwave
    /opt
)
message("DASM_INCLUDE_DIR is ${DASM_INCLUDE_DIR}")
set(DASM_INCLUDE_DIRS ${DASM_INCLUDE_DIR})

find_library(DASM_LIBRARY NAMES dasm libdasm
    HINTS
    $ENV{DASMDIR}
    PATH_SUFFIXES lib64 lib
    PATHS
    /sw
    /opt/local
    /opt/csw
    /opt
)
message("DASM_LIBRARY is ${DASM_LIBRARY}")
set(DASM_LIBRARIES ${DASM_LIBRARY})

INCLUDE(FindPackageHandleStandardArgs)

find_package_handle_standard_args(DASM REQUIRED_VARS DASM_LIBRARY DASM_INCLUDE_DIR)

mark_as_advanced(DASM_INCLUDE_DIR DASM_LIBRARY)
