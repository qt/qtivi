import QtQuick 2.0

Item {

    property int intProperty: testObject.intAttributeProperty.value
    property int enumProperty: testObject.enumAttributeProperty.value

    function intTest() {
        testObject.intAttributeProperty.value = "foo"
    }

    function enumTest() {
        testObject.enumAttributeProperty.value = 50
    }
}
