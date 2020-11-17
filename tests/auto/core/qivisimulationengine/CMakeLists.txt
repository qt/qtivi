# Generated from qivisimulationengine.pro.

#####################################################################
## tst_qivisimulationengine Test:
#####################################################################

qt_add_test(tst_qivisimulationengine
    SOURCES
        tst_qivisimulationengine.cpp
    PUBLIC_LIBRARIES
        Qt::Gui
        Qt::IviCore
        Qt::IviCorePrivate
        Qt::Qml
)

# Resources:
set(resource_resource_files
    "invalid-data.json"
    "simple.json"
    "simple.qml"
)

qt_add_resource(tst_qivisimulationengine "resource"
    PREFIX
        "/"
    FILES
        ${resource_resource_files}
)


#### Keys ignored in scope 1:.:.:qivisimulationengine.pro:<TRUE>:
# QMAKE_PROJECT_NAME = "$$TARGET"
# TEMPLATE = "app"
