/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
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

import QtQuick 2.0
import TestObject 1.0

Item {

    property int intProperty: testObject.intAttributeProperty.value
    property int enumProperty: testObject.enumAttributeProperty.value
    property var flagsProperty: testObject.flagsAttributeProperty.value
    property var extendFlag: testObject.flagsAttributeProperty.value
    property bool isFlag2Set: testObject.flagsAttributeProperty.value & TestObject.TestFlag_2
    property bool isFlag3Set: testObject.flagsAttributeProperty.value & TestObject.TestFlag_3

    function updateValues() {
        testObject.intAttributeProperty.value = 2.4
        testObject.enumAttributeProperty.value = TestObject.TestValue_2
        testObject.flagsAttributeProperty.value = TestObject.TestFlag_1 | TestObject.TestFlag_2
        extendFlag = testObject.flagsAttributeProperty.value | TestObject.TestFlag_3
    }
}
