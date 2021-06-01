/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
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

static QString longMsg = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy "
"eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et "
"accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est "
"Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam "
"nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero "
"eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata anctu "
"sest Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr,sed diam "
"nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero "
"eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimta sanctu "
"sest Lorem ipsum dolor sit amet. Duis autem vel eum iriure dolor in hendrerit in vulputate velit "
"esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et "
"iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugit null "
"afacilisi. Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nih euismod "
"tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veiam, quis nostru "
"dexerci dolor";

// Copied from dlt-daemon_cfg.h
// Defines how big the buffer for a msg payload needs to be to copy long msg
#define DLT_DAEMON_TEXTSIZE 10024

struct QDltMessage {
    QString appId;
    QString ctxId;
    QString payload;
};

class QDltParser {

public:

    QDltParser(QFile *file)
        : dltFile(new DltFile())
    {
        QVERIFY(dlt_file_init(dltFile, false) >= 0);
        QVERIFY(dlt_file_open(dltFile, file->fileName().toLocal8Bit(), false) >= 0);
    }

    ~QDltParser()
    {
        QVERIFY(dlt_file_close(dltFile, false) >= 0);
        QVERIFY(dlt_file_free(dltFile, false) >= 0);
        delete dltFile;
    }

    QString idToString(char *id)
    {
        QByteArray appData(DLT_ID_SIZE, '\0');
        dlt_print_id(appData.data(), id);
        return QString(appData);
    }

    QString readPayload(DltMessage *msg)
    {
        char payload[DLT_DAEMON_TEXTSIZE];
        dlt_message_payload(msg, payload, DLT_DAEMON_TEXTSIZE, DLT_OUTPUT_ASCII, false);
        payload[sizeof(payload) - 1] = 0;
        return QString::fromLocal8Bit(payload);
    }

    QList<QDltMessage> readNextMessages()
    {
        QList<QDltMessage> messageList;
        //Read the complete file
        while (dlt_file_read(dltFile, false) >= 0){};
        //Parse the messages
        for (int i = 0; i < dltFile->counter; i++) {
            dlt_file_message(dltFile, i, false);

            QDltMessage msg;
            msg.appId = idToString(dltFile->msg.extendedheader->apid);
            msg.ctxId = idToString(dltFile->msg.extendedheader->ctid);
            msg.payload = readPayload(&(dltFile->msg));
            messageList.append(msg);
        }

        return messageList;
    }

private:
    DltFile *dltFile;
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

    ~QDltTest()
    {
        delete m_dltParser;
    }

private Q_SLOTS:

    void initTestCase();
    void init();
    void testLongMessages_data();
    void testLogging();
    void testLongMessages();

private:
    QTemporaryFile m_tempFile;
    QDltParser *m_dltParser;
};

void QDltTest::initTestCase()
{
    qRegisterMetaType<QDltRegistration::LongMessageBehavior>();
    //Install the DLT message handler
    qInstallMessageHandler(QDltRegistration::messageHandler);
}

void QDltTest::init()
{
    //Reset the log file after each test
    m_tempFile.resize(0);
    //Initialize the DLT log file and write the header to it.
    dlt_init_file(m_tempFile.fileName().toLocal8Bit());
    //Create a new Parser (to start reading the file from the beginning)
    delete m_dltParser;
    m_dltParser = new QDltParser(&m_tempFile);
}

void QDltTest::testLogging()
{
    QDltRegistration *registration = globalDltRegistration();
    registration->registerApplication("APP1", "Description for APP");
    registration->registerCategory(&TEST1(), "TES1", "Test Category One");

    QString msg = QStringLiteral("TEST");
    QString expectedMsg = QString(QStringLiteral("%1: \"%2\"")).arg(TEST1().categoryName(), msg);

    qWarning(TEST1) << msg;

    QList<QDltMessage> dltMessages = m_dltParser->readNextMessages();
    QCOMPARE(dltMessages.count(), 1);
    QCOMPARE(dltMessages.at(0).appId, QLatin1String("APP1"));
    QCOMPARE(dltMessages.at(0).ctxId, QLatin1String("TES1"));
    QCOMPARE(dltMessages.at(0).payload, expectedMsg);
}

void QDltTest::testLongMessages_data()
{
    QTest::addColumn<QDltRegistration::LongMessageBehavior>("behavior");
    QTest::addColumn<QString>("msg");
    QTest::addColumn<QStringList>("expected_msgs");

    QTest::addRow("Truncate ASCII") << QDltRegistration::LongMessageBehavior::Truncate
                                    << longMsg + QStringLiteral("ipsum")
                                    << QStringList(longMsg + QStringLiteral("ipsu"));
    QTest::addRow("Truncate UTF-8 2 bytes") << QDltRegistration::LongMessageBehavior::Truncate
                                            << longMsg + QStringLiteral("©®¥¶¼")
                                            << QStringList(longMsg + QStringLiteral("©®"));
    QTest::addRow("Truncate UTF-8 3 bytes") << QDltRegistration::LongMessageBehavior::Truncate
                                            << longMsg + QStringLiteral("你好世界")
                                            << QStringList(longMsg +  QStringLiteral("你"));
    QTest::addRow("Split ASCII") << QDltRegistration::LongMessageBehavior::Split
                                 << longMsg + QStringLiteral("ipsum")
                                 << QStringList({longMsg + QStringLiteral("ipsu"), QStringLiteral("m\"")});
    QTest::addRow("Split UTF-8 2 bytes") << QDltRegistration::LongMessageBehavior::Split
                                         << longMsg + QStringLiteral("©®¥¶¼")
                                         << QStringList({longMsg + QStringLiteral("©®"), QStringLiteral("¥¶¼\"")});
    QTest::addRow("Split UTF-8 3 bytes") << QDltRegistration::LongMessageBehavior::Split
                                         << longMsg +  QStringLiteral("你好世界")
                                         << QStringList({longMsg +  QStringLiteral("你"),  QStringLiteral("好世界\"")});
    QTest::addRow("Pass ASCII") << QDltRegistration::LongMessageBehavior::Pass
                                << longMsg + QStringLiteral("ipsum123456789a")
                                << QStringList(QString());
    QTest::addRow("Pass UTF-8 2 bytes") << QDltRegistration::LongMessageBehavior::Pass
                                        << longMsg + QStringLiteral("©®¥¶¼")
                                        << QStringList(QString());
    QTest::addRow("Pass UTF-8 3 bytes") << QDltRegistration::LongMessageBehavior::Pass
                                        << longMsg +  QStringLiteral("你好世界")
                                        << QStringList(QString());
}

void QDltTest::testLongMessages()
{
    QFETCH(QDltRegistration::LongMessageBehavior, behavior);
    QFETCH(QString, msg);
    QFETCH(QStringList, expected_msgs);

    QDltRegistration *registration = globalDltRegistration();
    registration->registerApplication("APP1", "Description for APP");
    registration->registerCategory(&TEST1(), "TES1", "Test Category One");
    registration->setLongMessageBehavior(behavior);

    qWarning(TEST1) << msg;

    QList<QDltMessage> dltMessages = m_dltParser->readNextMessages();
    QCOMPARE(dltMessages.count(), expected_msgs.count());

    int i = 0;
    for (const QString &expected_msg : expected_msgs) {
        QString expectedMsg;
        //The logging category will be added before the splitting, it's only part of the first msg
        if (i == 0 && behavior != QDltRegistration::LongMessageBehavior::Pass) {
            //The closing quotes are part of the message pattern and will be cut as well.
            expectedMsg = QString(QStringLiteral("%1: \"%2")).arg(TEST1().categoryName(), expected_msg);
        } else {
            expectedMsg = expected_msg;
        }

        QCOMPARE(dltMessages.at(i).appId, QLatin1String("APP1"));
        QCOMPARE(dltMessages.at(i).ctxId, QLatin1String("TES1"));
        // Enable me to compare the full content on the console
        // std::cout << dltMessages.at(i).payload.toStdString() << "\n" << expectedMsg.toStdString() << std::endl;
        QCOMPARE(dltMessages.at(i).payload, expectedMsg);
        i++;
    }
}

QTEST_APPLESS_MAIN(QDltTest)

#include "tst_dlt.moc"

