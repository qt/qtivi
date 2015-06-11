/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

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
    registration->registerCategory(TEST1().categoryName(), "TES1", "Test Category One");

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

