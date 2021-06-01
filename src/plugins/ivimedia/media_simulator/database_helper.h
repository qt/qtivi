/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef DATABASE_HELPER_H
#define DATABASE_HELPER_H

#include <QByteArray>
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QTemporaryFile>
#include <QCoreApplication>

#include "logging.h"

QString mediaDatabaseFile()
{
    QString dbFile;
    const QByteArray database = qgetenv("QTIVIMEDIA_SIMULATOR_DATABASE");
    if (qEnvironmentVariableIsSet("QTIVIMEDIA_TEMPORARY_DATABASE")) {
        auto *tempFile = new QTemporaryFile(qApp);
        tempFile->open();
        dbFile = tempFile->fileName();
        qCInfo(media) << "QTIVIMEDIA_TEMPORARY_DATABASE environment variable is set.\n"
                    << "Using the temporary database: " << tempFile->fileName();
    } else if (!database.isEmpty()) {
        dbFile = QFile::decodeName(database);
        if (!QFileInfo(dbFile).isAbsolute())
            qCInfo(media) << "Please set an valid absolute path for QTIVIMEDIA_SIMULATOR_DATABASE. Current path:" << dbFile;
    } else {
        const QDir cacheLocation = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        if (!cacheLocation.exists())
            cacheLocation.mkpath(QStringLiteral("."));
        dbFile = cacheLocation.absoluteFilePath(QStringLiteral("ivimedia.db"));
        qCInfo(media) << "Used media database:" << dbFile;
    }
    return dbFile;
}

QSqlDatabase createDatabaseConnection(const QString &connectionName, const QString &dbFile)
{
    QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), connectionName);
    db.setDatabaseName(dbFile);
    if (!db.open())
        qFatal("Couldn't couldn't open database: %s", qPrintable(db.lastError().text()));
    return db;
}

void createMediaDatabase(const QString &dbFile)
{
    QSqlDatabase db = createDatabaseConnection(QStringLiteral("main"), dbFile);
    QSqlQuery query = db.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS \"queue\" (\"id\" INTEGER PRIMARY KEY, \"qindex\" INTEGER, \"track_index\" INTEGER)"));
    if (query.lastError().isValid())
        qFatal("Couldn't create Database Tables: %s", qPrintable(query.lastError().text()));

    query = db.exec("CREATE TABLE IF NOT EXISTS track "
                     "(id integer primary key, "
                     "trackName varchar(200), "
                     "albumName varchar(200), "
                     "artistName varchar(200), "
                     "genre varchar(200), "
                     "number integer,"
                     "file varchar(200),"
                     "coverArtUrl varchar(200),"
                     "UNIQUE(file))");

    if (query.lastError().isValid())
        qFatal("Couldn't create Database Tables: %s", qPrintable(query.lastError().text()));
    db.commit();
}

#endif // DATABASE_HELPER_H
