import QtQuick 2.0

Item {

    property int intProperty: testObject.intAttributeProperty.value
    property int enumProperty: testObject.enumAttributeProperty.value

    function updateValues() {
        testObject.intAttributeProperty.value = 2.4
        testObject.enumAttributeProperty.value = 2
    }
}
