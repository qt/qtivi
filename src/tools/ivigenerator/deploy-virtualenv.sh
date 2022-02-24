#!/bin/bash

#############################################################################
##
## Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
## Contact: https://www.qt.io/licensing/
##
## This file is part of the QtIvi module of the Qt Toolkit.
##
## $QT_BEGIN_LICENSE:GPL-EXCEPT-QTAS$
## Commercial License Usage
## Licensees holding valid commercial Qt Automotive Suite licenses may use
## this file in accordance with the commercial license agreement provided
## with the Software or, alternatively, in accordance with the terms
## contained in a written agreement between you and The Qt Company.  For
## licensing terms and conditions see https://www.qt.io/terms-conditions.
## For further information use the contact form at https://www.qt.io/contact-us.
##
## GNU General Public License Usage
## Alternatively, this file may be used under the terms of the GNU
## General Public License version 3 as published by the Free Software
## Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
## included in the packaging of this file. Please review the following
## information to ensure the GNU General Public License requirements will
## be met: https://www.gnu.org/licenses/gpl-3.0.html.
##
## $QT_END_LICENSE$
##
#############################################################################

SCRIPT=$(dirname $0)
usage()
{
     echo "deploy-virtualenv.sh <virtualenv>"
     exit 1
}

[ "$#" -lt 1 ] && usage
VIRTUALENV="${@: -1}"
[ ! -d "$VIRTUALENV" ] && usage
[ ! -d "$VIRTUALENV/lib" ] && usage
VIRTUALENV_LIB=$VIRTUALENV/lib
for file in "$VIRTUALENV_LIB"/python* ; do
    if [[ -d $file ]] ; then
        LIB_FOLDER=$file
        PYTHON_VERSION=$(basename "$file")
        break
    fi
done
[ ! -d "$LIB_FOLDER" ] && usage
if [[ -e "$LIB_FOLDER/orig-prefix.txt" ]] ; then
    ORIG_PREFIX=$(<"$LIB_FOLDER"/orig-prefix.txt)
else
    if [[  -e "$VIRTUALENV/pyvenv.cfg" ]] ; then
        ORIG_PREFIX=$(awk -F " = " '/base-prefix/ {print $2}' $VIRTUALENV/pyvenv.cfg)
    else
        echo "Neither $LIB_FOLDER/orig-prefix.txt or $VIRTUALENV/pyvenv.cfg exists";
        exit 1
    fi
fi

if [ "$(uname)" == "Darwin" ]; then
    PLATFORM="darwin"
else
    PLATFORM="linux"
fi

if [ "$PLATFORM" == "linux" ]; then
    # If the python executable has a dependency towards a libpython
    # copy the file and add it as LD_LIBRARY_PATH to the activate script
    LIBPYTHON=`ldd $VIRTUALENV/bin/python | awk '{print $3}' | grep python`
    if [[ -e "$LIBPYTHON" ]] ; then
       echo "copying $LIBPYTHON"
       cp -Lf "$LIBPYTHON" "$VIRTUALENV/bin"
    fi
fi

# Find all the locations used for the system python files
# They are located in prefix, but we don't know the sub-folder (it is lib on most systems, but lib64 on some others)
ORIG_LIBS=`$VIRTUALENV/bin/python3 -c "import sys;
import os;
prefix=os.path.realpath('${ORIG_PREFIX}')
for path in sys.path:
    if path.startswith(prefix):
        print (path)
"`

if [[ ! -e "$SCRIPT/deploy-virtualenv-files.txt" ]] ; then
    echo "$SCRIPT/deploy-virtualenv-files.txt doesn't exist";
    exit 1
fi

for ORIG_LIB in ${ORIG_LIBS} ; do
        echo "copying files from $ORIG_LIB to $LIB_FOLDER"
        FILES=$(<$SCRIPT/deploy-virtualenv-files.txt)
        for file in ${FILES} ; do
            expand_wildcard=($ORIG_LIB/$file)
            [ ! -e "$expand_wildcard" ] && continue;
            cp -RLfn "$ORIG_LIB"/$file "$LIB_FOLDER/"
        done
done

# random.py is needed in order to generate temp directories from python
# It is based on hashlib, which needs libcrypto and libssl to work.
# As there is no compatibility for openssl libs, we need to copy
# them to the bin folder similar to libpython
if [ "$PLATFORM" == "linux" ]; then
    #TODO This also needs to be fixed for mac

    HASHLIB=`find $LIB_FOLDER/lib-dynload -iname '_hashlib*'`
    if [[ -e "$HASHLIB" ]] ; then
        LIBCRYPTO=`ldd $HASHLIB | awk '{print $3}' | grep libcrypto`
        echo "copying $LIBCRYPTO"
        cp -Lf "$LIBCRYPTO" "$VIRTUALENV/bin"
        LIBSSL=`ldd $HASHLIB | awk '{print $3}' | grep libssl`
        echo "copying $LIBSSL"
        cp -Lf "$LIBSSL" "$VIRTUALENV/bin"
    fi
    CTYPESLIB=`find $LIB_FOLDER/lib-dynload -iname '_ctypes.*'`
    if [[ -e "$CTYPESLIB" ]] ; then
        LIBFFI=`ldd $CTYPESLIB | awk '{print $3}' | grep libffi`
        echo "copying $LIBFFI"
        cp -Lf "$LIBFFI" "$VIRTUALENV/bin"
    fi

else
    # Find the linked Python lib and its framework
    VERSION_STR=`$VIRTUALENV/bin/python -c "import sys; print(\"{0}.{1}\".format(sys.version_info.major, sys.version_info.minor))"`
    PYTHON_LIB_PATH=`otool -L $VIRTUALENV/bin/python | awk '{print $1}' | grep Python`
    PYTHON_FRAMEWORK_PATH=`otool -L $VIRTUALENV/bin/python | egrep -o '^.*Python.framework'`
    # Copy the framework into our virtualenv
    cp -a $PYTHON_FRAMEWORK_PATH $VIRTUALENV/bin
    # Delete the python folder from the framework as we already have that in our virtualenv
    rm -rf $VIRTUALENV/bin/Python.framework/Versions/$VERSION_STR/lib/$PYTHON_VERSION
    rm -rf $VIRTUALENV/bin/Python.framework/Versions/2*
    ln -sf $VERSION_STR $VIRTUALENV/bin/Python.framework/Versions/Current
    # Use the copied framework in the python binary
    install_name_tool -change $PYTHON_LIB_PATH @executable_path/Python.framework/Versions/Current/Python $VIRTUALENV/bin/python
    # And fix the reference of the Python.app inside the framework
    install_name_tool -change $PYTHON_LIB_PATH @loader_path/../../../../Python $VIRTUALENV/bin/Python.framework/Versions/Current/Resources/Python.app/Contents/MacOS/Python
fi

# some files might have wrong permissions, e.g. readonly
chmod -R ug+rw $VIRTUALENV

if [ "$(readlink -- "$VIRTUALENV/lib64")" != "lib" ] ; then
    cp -a "$VIRTUALENV/lib64"/* "$VIRTUALENV/lib/"
    rm -rf "$VIRTUALENV/lib64"
    cd "$VIRTUALENV"
    ln -s lib lib64
    cd -
fi

echo "done"
