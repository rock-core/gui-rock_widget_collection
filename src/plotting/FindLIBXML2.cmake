# - Try to find Eigen2 lib
# Once done this will define
#
#  EIGEN2_FOUND - system has eigen lib
#  EIGEN2_INCLUDE_DIR - the eigen include directory

# Copyright (c) 2006, 2007 Montel Laurent, <montel@kde.org>
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if (LIBXML2_INCLUDE_DIR)

  # in cache already
  set(LIBXML2_FOUND TRUE)

endif (LIBXML2_INCLUDE_DIR)

find_path(LIBXML2_INCLUDE_DIR NAMES xpath.h
     PATH_SUFFIXES libxml2/libxml
     HINTS
   )


SET(LIBXML2_NAMES libxml2)
FIND_LIBRARY(LIBXML2_LIBRARY NAMES ${LIBXML2_NAMES} libxml2.so)
MARK_AS_ADVANCED(LIBXML2_LIBRARY LIBXML2_INCLUDE_DIR )


MESSAGE(STATUS "Lib: " ${LIBXML2_LIBRARY})
MESSAGE(STATUS "Include: " ${LIBXML2_INCLUDE_DIR})

# Per-recommendation
SET(LIBXML2_INCLUDE_DIRS "${LIBXML2_INCLUDE_DIR}/.." "${LIBXML2_INCLUDE_DIR}")
SET(LIBXML2_LIBRARIES    "${LIBXML2_LIBRARY}")

# handle the QUIETLY and REQUIRED arguments and set ZLIB_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LIBXML2 DEFAULT_MSG LIBXML2_LIBRARIES LIBXML2_INCLUDE_DIRS)


