/****************************************************************************
**
** Copyright (C) 2015 Pelagicore AG
** Contact: http://www.pelagicore.com/
**
** This file is part of QtGeniviExtras.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial QtGeniviExtras licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Pelagicore. For licensing terms
** and conditions see http://www.pelagicore.com.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, Pelagicore gives you certain additional
** rights. These rights are described in Pelagicore LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QString>
#include <QtTest>
#include <QtGeniviExtras/QtDlt>
#include <QLoggingCategory>

#include <dlt/dlt.h>
#include <dlt/dlt_common.h>

#include <iostream>

Q_LOGGING_CATEGORY(TEST1, "qtgeniviextras.test1")


struct QDltMessage {
    QString appId;
    QString ctxId;
    QString payload;
};

class QDltParser {

public:

    QDltParser(QFile* file)
        : dltFile(new DltFile())
    {
        QVERIFY(dlt_file_init(dltFile, false) >= 0);
        QVERIFY(dlt_file_open(dltFile, file->fileName().toLocal8Bit(), false) >= 0);
    }

    ~QDltParser()
    {
        QVERIFY(dlt_file_close(dltFile, false) >= 0);
        QVERIFY(dlt_file_free(dltFile, false) >= 0);
    }

    QString idToString(char* id)
    {
        QByteArray appData(DLT_ID_SIZE, '\0');
        dlt_print_id(appData.data(),id);
        return QString(appData);
    }

    QString readPayload(DltMessage* msg)
    {
        QByteArray payload(1024, '\0');
        dlt_message_payload(msg,payload.data(),1024,DLT_OUTPUT_ASCII,false);
        return QString(payload);
    }

    QList<QDltMessage> readNextMessages()
    {
        QList<QDltMessage> messageList;
        while (dlt_file_read(dltFile, false) == 1)
        {
            dlt_file_message(dltFile, 0, false);

            QDltMessage msg;
            msg.appId = idToString(dltFile->msg.extendedheader->apid);
            msg.ctxId = idToString(dltFile->msg.extendedheader->ctid);
            msg.payload = readPayload(&(dltFile->msg));
            messageList.append(msg);
        }

        return messageList;
    }

private:
    DltFile* dltFile;
};


class QDltTest : public QObject
{
    Q_OBJECT

public:
    QDltTest()
    {
        m_tempFile.open();
        m_dltParser = new QDltParser(&m_tempFile);
    }

private Q_SLOTS:

    void initTestCase();
//    void registerApplication();
//    void registerTwoApplications();
//    void registerContexts();
    void testLogging();

private:
    QTemporaryFile m_tempFile;
    QDltParser* m_dltParser;
};

void QDltTest::initTestCase()
{
    //Initialize the dlt system before the QDltRegistration can do it to log to a file.
    dlt_init_file(m_tempFile.fileName().toLocal8Bit());

    //Install the DLT message handler
    qInstallMessageHandler(QDltRegistration::messageHandler);
}

void QDltTest::testLogging()
{
    QDltRegistration* registration = globalDltRegistration();
    registration->registerApplication("APP1", "Description for APP");
    registration->registerCategory(&TEST1(), "TES1", "Test Category One");

    QString msg = QLatin1Literal("TEST");
    QString expectedMsg = QString("%1: \"%2\"").arg(TEST1().categoryName()).arg(msg);

    qWarning(TEST1) << msg;

    QList<QDltMessage> dltMessages = m_dltParser->readNextMessages();
    QCOMPARE(dltMessages.count(), 1);
    QCOMPARE(dltMessages.at(0).appId, QLatin1Literal("APP1"));
    QCOMPARE(dltMessages.at(0).ctxId, QLatin1Literal("TES1"));
    QCOMPARE(dltMessages.at(0).payload, expectedMsg);
}


QTEST_APPLESS_MAIN(QDltTest)

#include "tst_dlt.moc"

