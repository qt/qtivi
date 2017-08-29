import QtQuick 2.7
import QtQuick.Controls 2.1

ComboBox {
    id: root

    property var value: 0

    FontMetrics {
        font: root.font
        id: _metrics
    }

    Binding {
        target: root
        property: "value"
        value: model.get(currentIndex).value
    }

    onValueChanged: {
        for(var i=0; i<model.count; i++) {
            if (model.get(i).value == value) {
                //Make the ComboBox as big as it's content
                root.implicitWidth = _metrics.boundingRect(model.get(i).key).width + 100
                currentIndex = i;
                return;
            }
        }
        console.warn("Couldn't find the value in the EnumControl model: " + value);
    }
}
