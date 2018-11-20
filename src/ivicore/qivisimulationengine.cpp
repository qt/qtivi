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

#include "qivisimulationengine.h"
#include <qivisimulationglobalobject_p.h>

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QDebug>
#include <QQmlContext>
#include <QLoggingCategory>

QT_BEGIN_NAMESPACE

/*!
    \class QIviSimulationEngine
    \inmodule QtIviCore
    \brief QIviSimulationEngine provides a way to script a simulation backend from QML.

    This class is an extended QQmlApplicationEngine which can be used to load QML files. It is made
    especially for \l {Dynamic Backend System}{simulation backends} to script the behavior of a
    simulation backend from QML.

    In contrast to a normal QQmlEngine the QIviSimulationEngine provides an extra template function
    called registerSimulationInstance().
    Using this function it is possible to register an class instance as a QML type. Inside a QML
    file this QML type can be used to define the behavior for function calls, update properties or
    emit signals.

    \section1 Registering an instance

    Any instance of a class derived from QObject can be registered to the QIviSimulationEngine by
    calling the registerSimulationInstance function. Similar to qmlRegisterTypes, the provided uri,
    version and name is used for importing the type from within QML.

    \code
    class MyClass : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(int currentTemperature READ currentTemperature WRITE setCurrentTemperature NOTIFY currentTemperatureChanged)

        ...
    }
    \endcode

    An instance of this simple class can be registered like this:

    \code
    QIviSimulationEngine engine;
    MyClass myClass;
    engine.registerSimulationInstance<MyClass>(&myClass, "Test", 1, 0, "MyClass");
    engine.loadSimulation("simulation.qml")
    \endcode

    The registered instance has the same constraints as other C++ classes exposed to QML and needs
    to use Q_PROPERTY, Q_INVOKABLE or slots to make the functionality available to QML.

    \section1 Using the type from QML

    Once an instance is registered to the engine, the type can be used like any other QML element
    in a declarative form:

    \qml
    import QtQuick 2.0
    import Test 1.0

    Item {
        MyClass {
            id: myClass

            Component.onCompleted: currentTemperature = 10;
        }

        Timer {
            running: true
            repeat: true
            interval: 1000
            onTriggered: myClass.currentTemperature++;
        }
    }
    \endqml

    This QML file will initialize the \c currentTemperature of \c myClass with a value of \e 10 and
    increase it every second.

    In the same way values can be updated from the C++ side and the QML side can react to the
    change. E.g. the following QML prints the \c currentTemperature whenever it changed:

    \qml
    import QtQuick 2.0
    import Test 1.0

    MyClass {
        onCurrentTemperatureChanged: print(currentTemperature)
    }
    \endqml

    The slot will be called once the myClass variable is updated:

    \code
    QIviSimulationEngine engine;
    MyClass myClass;
    engine.registerSimulationInstance<MyClass>(&myClass, "Test", 1, 0, "MyClass");
    engine.loadSimulation("simulation.qml")
    ...
    myClass.setCurrentTemperature(100);
    \endcode

    \section1 Forwarding calls from the instance to the engine

    Providing the behavior for invokable functions in QML can be done as well, but for this the
    exposed class needs to be extended.

    E.g. by adding the following line to the \c setCurrentTemperature setter:

    \code
    void MyClass::setCurrentTemperature(int currentTemperature)
    {
        QIVI_SIMULATION_TRY_CALL(MyClass, "setCurrentTemperature", void, currentTemperature);

        if (m_currentTemperature == currentTemperature)
            return;
        m_currentTemperature = currentTemperature;
        emit currentTemperatureChanged(m_currentTemperature);
    }
    \endcode

    Calling the setCurrentTemperature() function will now try to forward the call to the QML
    instance if a function matching the signature is defined in QML and when successful, use its
    returned value and skip the execution of the original C++ function.

    By using the following QML snippet the execution of the C++ setter is skipped and only an error
    is emitted on the console:

    \qml
    import QtQuick 2.0
    import Test 1.0

    MyClass {
        function setCurrentTemperature(temperature) {
            print("Updating the temperature is not possible")
        }
    }
    \endqml

    \section1 Reusing existing behavior in the instance

    Replacing the C++ functionality with a QML behavior is not always what is desired. Because of
    this, it is also possible to call the original C++ behavior from QML. For this the original C++
    function needs to be a Q_INVOKABLE or a slot and the functionality works like function
    overriding in C++. In the C++ world the functionality of the overridden function can be
    accessed by calling \c <BaseClass>::<function>. In the exposed QML type this is possible by
    calling the function in the \e Base object.

    \qml
    import QtQuick 2.0
    import Test 1.0

    MyClass {
        function setCurrentTemperature(temperature) {
            print("Updating the temperature: " + temperature )
            Base.setCurrentTemperature(temperature)
        }
    }
    \endqml

    This QML code overrides the setCurrentTemperature behavior in QML and prints an debug message
    for the new value. The original C++ behavior is called by using \c
    Base.setCurrentTemperature(temperature).

    \section1 Multiple QML instances

    The registered instance is exposed as a normal QML type. This makes it possible to have
    multiple declarations in QML and by that have multiple QML instances linked to the same C++
    instance. Updating and reacting to property changes and signal emissions is possible in all
    instances, but should be used with care as this can result in property update loops and other
    issues.

    Forwarding C++ function calls to QML is limited. Every call is forwarded to only one QML
    instance as the return value is used from this call. If multiple QML instances define the same
    method, the C++ call is always forwarded to the first registered QML instance.
*/

QIviSimulationEngine::QIviSimulationEngine(QObject *parent)
    : QQmlApplicationEngine (parent)
    , m_globalObject(new QIviSimulationGlobalObject)
{
    rootContext()->setContextProperty(QStringLiteral("IviSimulator"), m_globalObject);
}

void QIviSimulationEngine::loadSimulationData(const QString &dataFile)
{
    qCDebug(qLcIviSimulationEngine) << "loading SimulationData" << dataFile;

    QFile file(dataFile);
    if (!file.open(QFile::ReadOnly)) {
        qCCritical(qLcIviSimulationEngine, "Cannot open the simulation data file %s: %s", qPrintable(dataFile), qPrintable(file.errorString()));
        return;
    }

    QJsonParseError pe;
    QByteArray data = file.readAll();
    QJsonDocument document = QJsonDocument::fromJson(data, &pe);
    if (pe.error != QJsonParseError::NoError) {
        qCCritical(qLcIviSimulationEngine, "Error parsing the simulation data in %s: %s", qPrintable(dataFile), qPrintable(pe.errorString()));
        qCCritical(qLcIviSimulationEngine, "Error context:\n %s", data.mid(qMax(pe.offset - 20, 0), 40).data());
    }
    m_globalObject->setSimulationData(document.toVariant());
}

/*!
    Loads the QML \a file as the simulation behavior.

    In addition to QQmlApplicationEngine::load(), this function provides functionality to change
    the used simulation file by using an environment variable.
*/
void QIviSimulationEngine::loadSimulation(const QUrl &file)
{
    load(file);
}

/*!
    \fn template <typename T> void QIviSimulationEngine::registerSimulationInstance(T* instance, const char *uri, int versionMajor, int versionMinor, const char *qmlName)

    Registers the provided \a instance in the QML system with the name \a qmlName, in the library
    imported from \a uri having the version number composed from \a versionMajor and \a
    versionMinor.

    \note The registered instance is only available to this instance of the QIviSimulationEngine.
    Using it from another QIviSimulationEngine or a QQmlEngine will not work and produce an error.

    \sa qmlRegisterType
*/

/*!
    \macro QIVI_SIMULATION_TRY_CALL_FUNC(instance_type, function, ret_func, ...)
    \relates QIviSimulationEngine

    Tries to call \a function in the QML instances registered for the instance of type \a
    instance_type. The variadic arguments are passed as arguments to the function in QML.

    If the call was successful the code passed in \a ret_func will be executed. This can be useful
    for situations when the return value needs to be converted first. The original return value is
    available as \c return_value.

    \code
    QIVI_SIMULATION_TRY_CALL_FUNC(MyClass, "contactList", return return_value.toStringList());
    \endcode

    \sa QIVI_SIMULATION_TRY_CALL {Forwarding calls from the instance to the engine}
*/

/*!
    \macro QIVI_SIMULATION_TRY_CALL(instance_type, function, ret_type, ...)
    \relates QIviSimulationEngine

    Tries to call \a function in the QML instances registered for the instance of type \a
    instance_type. The variadic arguments are passed as arguments to the function in QML.

    If the call was successful the return value of type \a ret_type will be returned and all code
    after this macro will \b not be executed.

    \sa QIVI_SIMULATION_TRY_CALL_FUNC {Forwarding calls from the instance to the engine}
*/

QT_END_NAMESPACE
