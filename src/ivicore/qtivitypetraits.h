/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
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

#ifndef QIVITYPETRAITS_H
#define QIVITYPETRAITS_H

#include <QtCore/qtypetraits.h>

QT_BEGIN_NAMESPACE

//This is a copy of specific typtraits from qtypetraits.h which were disabled for MSVC
//We copy it to enable it by default to not hardly depend on a specific Qt version here and also
//make it work with older Qt versions on selected platforms

namespace QtPrivate {
    // This functions checks whether the type has a implicit cast operator to int
    template <typename From, typename To>
    struct QIviConvertHelper {
      static small_ Test(To);
      static big_ Test(...);
      static From Create();
    };

    // Inherits from true_type if From is convertible to To, false_type otherwise.
    template <typename From, typename To>
    struct qtivi_is_convertible
        : integral_constant<bool,
                            sizeof(QIviConvertHelper<From, To>::Test(
                                      QIviConvertHelper<From, To>::Create()))
                            == sizeof(small_)> {
    };

    namespace qtivi_internal {

    template <class T> struct is_class_or_union {
      template <class U> static small_ tester(void (U::*)());
      template <class U> static big_ tester(...);
      static const bool value = sizeof(tester<T>(0)) == sizeof(small_);
    };

    // is_convertible chokes if the first argument is an array. That's why
    // we use add_reference here.
    template <bool NotUnum, class T> struct is_enum_impl
        : qtivi_is_convertible<typename add_reference<T>::type, int> { };

    template <class T> struct is_enum_impl<true, T> : false_type { };

    }  // namespace qtivi_internal

    // Specified by TR1 [4.5.1] primary type categories.

    // Implementation note:
    //
    // Each type is either void, integral, floating point, array, pointer,
    // reference, member object pointer, member function pointer, enum,
    // union or class. Out of these, only integral, floating point, reference,
    // class and enum types are potentially convertible to int. Therefore,
    // if a type is not a reference, integral, floating point or class and
    // is convertible to int, it's a enum. Adding cv-qualification to a type
    // does not change whether it's an enum.
    //
    // Is-convertible-to-int check is done only if all other checks pass,
    // because it can't be used with some types (e.g. void or classes with
    // inaccessible conversion operators).
    template <class T> struct qtivi_is_enum
        : qtivi_internal::is_enum_impl<
              is_same<T, void>::value ||
                  is_integral<T>::value ||
                  is_floating_point<T>::value ||
                  is_reference<T>::value ||
                  qtivi_internal::is_class_or_union<T>::value,
              T> { };

    template <class T> struct qtivi_is_enum<const T> : qtivi_is_enum<T> { };
    template <class T> struct qtivi_is_enum<volatile T> : qtivi_is_enum<T> { };
    template <class T> struct qtivi_is_enum<const volatile T> : qtivi_is_enum<T> { };


    template <class T> struct is_flag
        : integral_constant<bool,
            qtivi_is_convertible<T, int>::value &&
            !is_pod<T>::value &&
            !qtivi_is_enum<T>::value
          >{};
}

QT_END_NAMESPACE

#endif // QIVITYPETRAITS_H
