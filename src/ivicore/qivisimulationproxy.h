/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
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

#ifndef QIVISIMULATIONPROXY_H
#define QIVISIMULATIONPROXY_H

#include <QtIviCore/QtIviCoreModule>

#include <QtCore/QObject>
#include <QtCore/QLoggingCategory>
#include <QtCore/QVariant>
#include <QtCore/QMetaObject>
#include <QtQml/QQmlParserStatus>

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(qLcIviSimulationEngine)

class QIviSimulationEngine;

// The classes here can't be moved to a private header as they are used in macros in the user code
// They are still considered private as they shouldn't be used directly by the user.
namespace qtivi_private {

    // This is needed as QVariant doesn't support returning void
    // It is used to cast the variant to the needed return type and use it in the return statement.
    template <typename T> struct QIviReturnValueHelper {
        static T value(const QVariant &var)
        {
            return var.value<T>();
        }
    };

    template <> struct QIviReturnValueHelper <void> {
        static void value(const QVariant &var)
        {
            Q_UNUSED(var);
            return;
        }
    };

    class Q_QTIVICORE_EXPORT QIviSimulationProxyBase : public QObject, public QQmlParserStatus
    {
        Q_INTERFACES(QQmlParserStatus)

    public:
        QIviSimulationProxyBase(QMetaObject *staticMetaObject, QObject *instance, const QHash<int, int> &methodMap, QObject *parent=nullptr);

        virtual const QMetaObject *metaObject() const override;
        virtual void *qt_metacast(const char *classname) override;
        virtual int qt_metacall(QMetaObject::Call call, int methodId, void **a) override;

        void classBegin() override;
        void componentComplete() override;

        typedef void (*StaticMetacallFunction)(QObject *, QMetaObject::Call, int, void **);
        static QMetaObject buildObject(const QMetaObject *metaObject, QHash<int, int> &methodMap, QIviSimulationProxyBase::StaticMetacallFunction metaCallFunction);

        bool callQmlMethod(const char* function,
                          QGenericReturnArgument ret,
                          QGenericArgument val0 = QGenericArgument(nullptr),
                          QGenericArgument val1 = QGenericArgument(),
                          QGenericArgument val2 = QGenericArgument(),
                          QGenericArgument val3 = QGenericArgument(),
                          QGenericArgument val4 = QGenericArgument(),
                          QGenericArgument val5 = QGenericArgument(),
                          QGenericArgument val6 = QGenericArgument(),
                          QGenericArgument val7 = QGenericArgument(),
                          QGenericArgument val8 = QGenericArgument(),
                          QGenericArgument val9 = QGenericArgument());

        template<typename... Ts>
        bool callQmlMethod(const char* function, QVariant &returnValue, Ts... args)
        {
            return QIviSimulationProxyBase::callQmlMethod(function, Q_RETURN_ARG(QVariant, returnValue), Q_ARG(QVariant, QVariant::fromValue(args))...);
        }

    protected:
        void setup(QIviSimulationEngine *engine);

    private:
        bool m_noSimulationEngine;
        QObject *m_instance;
        QMetaObject *m_staticMetaObject;
        QHash<int, int> m_methodMap;
    };

    template <typename T> class QIviSimulationProxy: public QIviSimulationProxyBase
    {
    public:
        QIviSimulationProxy(QObject *p=nullptr)
            : QIviSimulationProxyBase(&staticMetaObject, m_instance, methodMap(), p)
        {
            Q_ASSERT_X(m_instance, "QIviSimulationProxy()", "QIviSimulationProxy::registerInstance needs to be called first");
        }

        ~QIviSimulationProxy()
        {
            proxies.removeAll(this);
        }

        void classBegin() override
        {
            QIviSimulationProxyBase::setup(m_engine);
            proxies.append(this);
        }

        // Function is used from QML when reading a property. The static QMetaObject has this function set
        // as the handler for all static meta calls
        static void qt_static_metacall(QObject *obj, QMetaObject::Call call, int methodId, void **a)
        {
            if (!obj)
                return;
            Q_ASSERT_X(m_instance, "qt_static_metacall()", "QIviSimulationProxy::registerInstance needs to be called first");
            // As the class acts as a proxy, forward all calls here to the registered instance
            // The methodIds start at 0 for the first property of this class. We need to add the
            // offset to get the absolute property index for the normal qt_metacall
            if (call == QMetaObject::ReadProperty || call == QMetaObject::WriteProperty) {
                obj->qt_metacall(call, methodId + staticMetaObject.propertyOffset(), a);
                return;
            }

            obj->qt_metacall(call, methodId, a);
        }

        static void registerInstance(QIviSimulationEngine *engine, T *instance)
        {
            Q_ASSERT_X(staticMetaObject.d.data, "registerInstance", "QIviSimulationProxy::buildMetaObject needs to be called first");

            m_engine = engine;
            m_instance = instance;
        }

        static QHash<int, int> &methodMap()
        {
            static QHash<int, int> map;
            return map;
        }

        static void buildMetaObject()
        {
            if (!staticMetaObject.d.data)
                staticMetaObject = QIviSimulationProxy<T>::buildObject(&T::staticMetaObject, QIviSimulationProxy<T>::methodMap(), &QIviSimulationProxy<T>::qt_static_metacall);
        }

        static QMetaObject staticMetaObject;
        static QList<QIviSimulationProxy<T> *> proxies;

    private:
        static QIviSimulationEngine *m_engine;
        static T *m_instance;
    };

    template <typename T> QMetaObject QIviSimulationProxy<T>::staticMetaObject = QMetaObject();
    template <typename T> T *QIviSimulationProxy<T>::m_instance = nullptr;
    template <typename T> QIviSimulationEngine *QIviSimulationProxy<T>::m_engine = nullptr;
    template <typename T> QList<QIviSimulationProxy<T> *> QIviSimulationProxy<T>::proxies =  QList<QIviSimulationProxy<T> *>();
}

#define QIVI_SIMULATION_TRY_CALL_FUNC(instance_type, function, ret_func, ...) \
for (auto _qivi_instance : qtivi_private::QIviSimulationProxy<instance_type>::proxies) { \
    QVariant return_value; \
    if (_qivi_instance->callQmlMethod(function, return_value, ##__VA_ARGS__)) { \
        ret_func; \
    } \
} \


#define QIVI_SIMULATION_TRY_CALL(instance_type, function, ret_type, ...) \
QIVI_SIMULATION_TRY_CALL_FUNC(instance_type, function, return qtivi_private::QIviReturnValueHelper<ret_type>::value(return_value);, ##__VA_ARGS__) \

QT_END_NAMESPACE

#endif // QIVISIMULATIONPROXY_H
