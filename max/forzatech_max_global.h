#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(FORZATECH_MAX_LIB)
#  define FORZATECH_MAX_EXPORT Q_DECL_EXPORT
# else
#  define FORZATECH_MAX_EXPORT Q_DECL_IMPORT
# endif
#else
# define FORZATECH_MAX_EXPORT
#endif
