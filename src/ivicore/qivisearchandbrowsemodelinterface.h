/****************************************************************************
**
** Copyright (C) 2017 Pelagicore AG
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

#ifndef QIVISEARCHMODELINTERFACE_H
#define QIVISEARCHMODELINTERFACE_H

#include <QObject>
#include <QSet>
#include <QMetaObject>
#include <QMetaProperty>
#include <QUuid>
#include <QtIviCore/QIviSearchAndBrowseModelItem>
#include <QtIviCore/QIviSearchAndBrowseModel>
#include <QtIviCore/QIviAbstractQueryTerm>

QT_BEGIN_NAMESPACE

class QIviSearchAndBrowseModelInterfacePrivate;

class Q_QTIVICORE_EXPORT QIviSearchAndBrowseModelInterface : public QObject
{
    Q_OBJECT

public:
    QIviSearchAndBrowseModelInterface(QObject *parent = Q_NULLPTR);
    virtual ~QIviSearchAndBrowseModelInterface();

    virtual QSet<QString> availableContentTypes() const;
    virtual QSet<QString> supportedIdentifiers(const QString &contentType) const;

    virtual void fetchData(const QUuid &identifier, const QString &type, QIviAbstractQueryTerm *term, const QList<QIviOrderTerm> &orderTerms, int start, int count) = 0;
    virtual bool canGoBack(const QUuid &identifier, const QString &type) = 0;
    virtual QString goBack(const QUuid &identifier, const QString &type) = 0;  // Only used when in-model navigation
    //TODO pass also an pointer here instead of the id ?
    virtual bool canGoForward(const QUuid &identifier, const QString &type, const QString &itemId) = 0; //Every Item has a id property which is filled by the backend implementation.
    virtual QString goForward(const QUuid &identifier, const QString &type, const QString &itemId) = 0; //Returns the new type identifier used for the next level. The identifier will stay the same for the following calls but the type might differ.

    virtual void insert(const QUuid &identifier, const QString &type, int index, const QIviSearchAndBrowseModelItem *item) = 0;
    virtual void remove(const QUuid &identifier, const QString &type, int index) = 0;
    virtual void move(const QUuid &identifier, const QString &type, int currentIndex, int newIndex) = 0;
    virtual int indexOf(const QUuid &identifier, const QString &type, const QIviSearchAndBrowseModelItem *item) = 0;

Q_SIGNALS:
    void supportedCapabilitiesChanged(const QUuid &identifier, QIviSearchAndBrowseModel::Capabilities capabilities);
    void countChanged(const QUuid &identifier, int newLength);                          // Emitted by the backend if it already knows the total count of items in the model (can be used by the dataChanged display method)
    void dataFetched(const QUuid &identifier, const QList<QVariant> &data, int start, bool moreAvailable);
    void dataChanged(const QUuid &identifier, const QList<QVariant> &data, int start, int count);    //start and count defines which data gets replace by the new data content. If data is empty the rows will be removed, if count is 0, the data will be added.
    void indexOfCallResult(const QUuid &identifier, int callID, int index);

protected:
    template <typename T>
    void registerContentType(const QString &contentType) {
        registerContentType(T::staticMetaObject, contentType);
    }
    void registerContentType(const QMetaObject &object, const QString &contentType);

private:
    Q_DECLARE_PRIVATE(QIviSearchAndBrowseModelInterface)
};

#define QIviSearchAndBrowseModel_iid "org.qt-project.qtivi.SearchAndBrowseModel/1.0"

QT_END_NAMESPACE

#endif // QIVISEARCHMODELINTERFACE_H
