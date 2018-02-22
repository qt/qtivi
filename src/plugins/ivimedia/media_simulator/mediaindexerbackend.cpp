/****************************************************************************
**
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
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
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#include "mediaindexerbackend.h"
#include "logging.h"

#include <QtConcurrent/QtConcurrent>

#include <QDirIterator>
#include <QImage>
#include <QSqlError>
#include <QSqlQuery>
#include <QtDebug>
#include <QThreadPool>

#ifndef QTIVI_NO_TAGLIB
#include <attachedpictureframe.h>
#include <fileref.h>
#include <id3v2frame.h>
#include <id3v2header.h>
#include <id3v2tag.h>
#include <mpegfile.h>
#include <tag.h>
#include <taglib.h>
#include <tstring.h>
#endif

MediaIndexerBackend::MediaIndexerBackend(const QSqlDatabase &database, QObject *parent)
    : QIviMediaIndexerControlBackendInterface(parent)
    , m_db(database)
    , m_state(QIviMediaIndexerControl::Idle)
    , m_threadPool(new QThreadPool(this))
{
    m_threadPool->setMaxThreadCount(1);

    connect(&m_watcher, SIGNAL(finished()), this, SLOT(onScanFinished()));

    QString mediaFolder = QDir::homePath() + QLatin1String("/media");
    const QByteArray customMediaFolder = qgetenv("QTIVIMEDIA_SIMULATOR_LOCALMEDIAFOLDER");
    if (customMediaFolder.isEmpty())
        qCCritical(media) << "QTIVIMEDIA_SIMULATOR_LOCALMEDIAFOLDER environment variable is not set, falling back to:" << mediaFolder;
    else
        mediaFolder = customMediaFolder;

#ifdef QTIVI_NO_TAGLIB
    qCCritical(media) << "The indexer simulation doesn't work without an installed taglib";
#endif

    //We want to have the indexer running also when the Indexing interface is not used.
    addMediaFolder(mediaFolder);
}

void MediaIndexerBackend::initialize()
{
    emit stateChanged(m_state);
    emit initializationDone();
}

void MediaIndexerBackend::pause()
{
    static const QLatin1String error("SIMULATION: Pausing the indexing is not supported");
    qCWarning(media) << error;
    emit errorChanged(QIviAbstractFeature::InvalidOperation, error);
}

void MediaIndexerBackend::resume()
{
    static const QLatin1String error("SIMULATION: Resuming the indexing is not supported");
    qCWarning(media) << error;
    emit errorChanged(QIviAbstractFeature::InvalidOperation, error);
}

void MediaIndexerBackend::addMediaFolder(const QString &path)
{
    ScanData data;
    data.remove = false;
    data.folder = path;
    m_folderQueue.append(data);

    scanNext();
}

void MediaIndexerBackend::removeMediaFolder(const QString &path)
{
    ScanData data;
    data.remove = true;
    data.folder = path;
    m_folderQueue.append(data);

    scanNext();
}

bool MediaIndexerBackend::scanWorker(const QString &mediaDir, bool removeData)
{
    setState(QIviMediaIndexerControl::Active);

    if (removeData) {
        qCInfo(media) << "Removing content: " << mediaDir;
        QSqlQuery query(m_db);

        bool ret = query.exec(QString("DELETE from track WHERE file LIKE '%1%'").arg(mediaDir));

        if (!ret) {
            setState(QIviMediaIndexerControl::Error);
            sqlError(this, query.lastQuery(), query.lastError().text());
            return false;
        }

        return true;
    }

    qCInfo(media) << "Scanning path: " << mediaDir;

    QStringList mediaFiles;
    mediaFiles << "*.mp3";

    QVector<QString> files;
    QDirIterator it(mediaDir, mediaFiles, QDir::Files, QDirIterator::Subdirectories);
    qCInfo(media) << "Calculating total file count";

    int totalFileCount = 0;
    while (it.hasNext()) {
        files.append(it.next());
        totalFileCount++;
    }
    qCInfo(media) << "total files: " << totalFileCount;
    int currentFileIndex = 0;
    for (const QString &fileName : files) {
        qCInfo(media) << "Processing file:" << fileName;

        if (qApp->closingDown())
            return false;

        QString defaultCoverArtUrl = fileName + QLatin1Literal(".png");
        QString coverArtUrl;
#ifndef QTIVI_NO_TAGLIB
        TagLib::FileRef f(TagLib::FileName(QFile::encodeName(fileName)));
        if (f.isNull())
            continue;
        QString trackName = QLatin1String(f.tag()->title().toCString());
        QString albumName = QLatin1String(f.tag()->album().toCString());
        QString artistName = QLatin1String(f.tag()->artist().toCString());
        QString genre = QLatin1String(f.tag()->genre().toCString());
        int number = f.tag()->track();

        // Extract cover art
        if (fileName.endsWith(QLatin1String("mp3"))) {
            TagLib::MPEG::File *file = static_cast<TagLib::MPEG::File*>(f.file());
            TagLib::ID3v2::Tag *tag = file->ID3v2Tag(true);
            TagLib::ID3v2::FrameList frameList = tag->frameList("APIC");

            if (frameList.isEmpty()) {
                qCWarning(media) << "No cover art was found";
            } else {
                TagLib::ID3v2::AttachedPictureFrame *coverImage =
                    static_cast<TagLib::ID3v2::AttachedPictureFrame *>(frameList.front());

                QImage coverQImg;
                coverArtUrl = defaultCoverArtUrl;

                coverQImg.loadFromData((const uchar *)coverImage->picture().data(), coverImage->picture().size());
                coverQImg.save(coverArtUrl, "PNG");
            }
        }

        QSqlQuery query(m_db);

        query.prepare("INSERT OR IGNORE INTO track (trackName, albumName, artistName, genre, number, file, coverArtUrl) "
                      "VALUES (:trackName, :albumName, :artistName, :genre, :number, :file, :coverArtUrl)");

        query.bindValue(":trackName", trackName);
        query.bindValue(":albumName", albumName);
        query.bindValue(":artistName", artistName);
        query.bindValue(":genre", genre);
        query.bindValue(":number", number);
        query.bindValue(":file", fileName);
        query.bindValue(":coverArtUrl", coverArtUrl);

        bool ret = query.exec();

        if (!ret) {
            setState(QIviMediaIndexerControl::Error);
            sqlError(this, query.lastQuery(), query.lastError().text());
            return false;
        } else {
            emit progressChanged(qreal(currentFileIndex)/qreal(totalFileCount));
        }
#else
        emit progressChanged(qreal(currentFileIndex)/qreal(totalFileCount));
#endif // QTIVI_NO_TAGLIB

        currentFileIndex++;
    }

    return true;
}

void MediaIndexerBackend::onScanFinished()
{
    if (!m_folderQueue.isEmpty()) {
        scanNext();
        return;
    }

    qCInfo(media) << "Scanning done";
#ifdef QTIVI_NO_TAGLIB
    qCCritical(media) << "No data was added, this is just a simulation";
#endif
    emit progressChanged(1);
    emit indexingDone();


    //If the last run didn't succeed we will stay in the Error state
    if (m_watcher.future().result())
        setState(QIviMediaIndexerControl::Idle);
}

void MediaIndexerBackend::scanNext()
{
    if (m_watcher.isRunning())
        return;

    ScanData data = m_folderQueue.dequeue();
    m_currentFolder = data.folder;
    m_watcher.setFuture(QtConcurrent::run(this, &MediaIndexerBackend::scanWorker, m_currentFolder, data.remove));
}

void MediaIndexerBackend::setState(QIviMediaIndexerControl::State state)
{
    m_state = state;
    emit stateChanged(state);
}
