import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1

RowLayout {
    id: root
    property alias model: repeater.model
    property int value
    property string textRole

    spacing: 2

    Repeater {
        id: repeater
        RowLayout {
            Text { text: model[textRole] }
            CheckBox {
                id: checkBox
                checked: root.value & model.value;
                onClicked: {
                    if (checked)
                        root.value |= model.value
                     else
                        root.value &= ~model.value
                }
            }
        }
    }
}
