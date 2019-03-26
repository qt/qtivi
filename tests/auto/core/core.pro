TEMPLATE = subdirs

SUBDIRS = servicemanagertest \
          qiviabstractfeature \
          qivipendingreply \
          queryparser \
          qivipagingmodel \
          qivisearchandbrowsemodel \
          qivisimulationengine \

QT_FOR_CONFIG += ivicore
qtConfig(ivigenerator): SUBDIRS += ivigenerator
