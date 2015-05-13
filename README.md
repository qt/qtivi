Building
=============
$ qmake
$ make

Building Documentation
=============
$ qmake
$ make docs

Building with Examples
=============
$ qmake CONFIG+=enable-examples
$ make

Using QtIVI without 'make install'
=============

In order to run the provided examples you need to setup the following environment variables:

Linux
========

export LD_LIBRARY_PATH=<build-directory>/lib
export QML2_IMPORT_PATH=<build-directory>/qml
export QT_PLUGIN_PATH=<build-directory>/plugins

MacOS
========

export DYLD_FRAMEWORK_PATH=<build-directory>/lib
export QML2_IMPORT_PATH=<build-directory>/qml
export QT_PLUGIN_PATH=<build-directory>/plugins
