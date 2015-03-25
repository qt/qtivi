#ifndef QTIVIGLOBAL_H
#define QTIVIGLOBAL_H

#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#  if defined(QT_BUILD_QTIVICORE_LIB)
#    define Q_QTIVICORE_EXPORT Q_DECL_EXPORT
#  else
#    define Q_QTIVICORE_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define Q_QTIVICORE_EXPORT
#endif

QT_END_NAMESPACE

#endif // QTIVIGLOBAL_H

