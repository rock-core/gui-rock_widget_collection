# - Try to find Eigen2 lib
# Once done this will define
#
#  EIGEN2_FOUND - system has eigen lib
#  EIGEN2_INCLUDE_DIR - the eigen include directory

# Copyright (c) 2006, 2007 Montel Laurent, <montel@kde.org>
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if(QWT_INCLUDE_DIR)
    # in cache already
    set(QWT_FOUND TRUE)
endif(QWT_INCLUDE_DIR)

SET(QWT_NAMES qwt-qt4 qwt)
FIND_LIBRARY(QWT_LIBRARY NAMES ${QWT_NAMES} PATHS /usr/lib /usr/lib/qwt)
find_path(QWT_INCLUDE_DIR NAMES qwt.h
    PATH_SUFFIXES qwt-qt4 qwt5 qwt
    HINTS /usr/local/Frameworks
    )
MARK_AS_ADVANCED(QWT_LIBRARY QWT_INCLUDE_DIR )

# Per-recommendation
SET(QWT_INCLUDE_DIRS ${QWT_INCLUDE_DIR})
SET(QWT_LIBRARIES    ${QWT_LIBRARY})

# handle the QUIETLY and REQUIRED arguments and set ZLIB_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(QWT DEFAULT_MSG QWT_LIBRARIES QWT_INCLUDE_DIRS)
