TEMPLATE = subdirs

include($$QTIVI_BUILD_ROOT/src/ivicore/qtivicore-config.pri)
QT_FOR_CONFIG += ivicore ivicore-private
qtConfig(ivigenerator): !qtConfig(system-ivigenerator): SUBDIRS += features
