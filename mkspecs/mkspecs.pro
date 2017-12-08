TEMPLATE = subdirs

include($$QTIVI_BUILD_ROOT/src/ivicore/qtivicore-config.pri)
QT_FOR_CONFIG += ivicore ivicore-private

# The features always need to be installed (also in the system-ivigenerator case)
# as yocto uses the mkspecs folder in the target sysroot, although the ivigenerator
# is in the native sysroot
qtConfig(ivigenerator): SUBDIRS += features
