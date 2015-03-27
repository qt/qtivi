/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

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

