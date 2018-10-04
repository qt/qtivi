TEMPLATE = subdirs

SUBDIRS = servicemanagertest \
          qivipropertyattribute \
          qiviproperty \
          qiviabstractfeature \
          qivipendingreply \
          queryparser \
          qivipagingmodel \
          qivisearchandbrowsemodel \
          qivisimulationengine \

QT_FOR_CONFIG += ivicore
qtConfig(ivigenerator): SUBDIRS += ivigenerator
