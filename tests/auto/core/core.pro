TEMPLATE = subdirs

SUBDIRS = servicemanagertest \
          qivipropertyattribute \
          qiviproperty \
          qiviabstractfeature \
          qivipendingreply \
          queryparser \
          qivipagingmodel \
          qivisearchandbrowsemodel \

QT_FOR_CONFIG += ivicore
qtConfig(ivigenerator): SUBDIRS += ivigenerator
