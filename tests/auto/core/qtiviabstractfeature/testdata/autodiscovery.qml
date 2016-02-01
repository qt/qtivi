import QtQuick 2.0
import testfeature 1.0

Item {

    TestFeature {
        objectName: "default"
    }

    TestFeature {
        objectName: "autoDiscoveryDisabled"
        autoDiscovery: false
    }
}
