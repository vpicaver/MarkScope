#ifndef MARKSCOPE_GLOBAL_H
#define MARKSCOPE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MARKSCOPE_LIBRARY)
#define MARKSCOPE_EXPORT Q_DECL_EXPORT
#else
#define MARKSCOPE_EXPORT Q_DECL_IMPORT
#endif

#endif // MARKSCOPE_GLOBAL_H
