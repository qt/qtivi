/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

#ifndef QGENIVIEXTRASGLOBAL_H
#define QGENIVIEXTRASGLOBAL_H

#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE

#if defined(QT_BUILD_GENIVIEXTRAS_LIB)
#  define Q_GENIVIEXTRAS_EXPORT Q_DECL_EXPORT
#else
#  define Q_GENIVIEXTRAS_EXPORT Q_DECL_IMPORT
#endif

QT_END_NAMESPACE

#endif // QGENIVIEXTRASGLOBAL_H
