TEMPLATE = subdirs

SUBDIRS = servicemanagertest \
          qivipropertyattribute \
          qiviproperty \
          qiviabstractfeature \
          qivipendingreply \
          queryparser \
          qivisearchandbrowsemodel \

QT_FOR_CONFIG += ivicore
qtConfig(ivigenerator): SUBDIRS += ivigenerator
