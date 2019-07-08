import QtQuick 2.0
import QtMultimedia 5.0


Item {
    id: propertyButton

    // current value would be the last one previously selected
    property alias value : popup.currentValue

    // model - contains the data and its structure.
    // note: this is the current selected item (filter) delegated to the GridView in popup
    property alias model : popup.model

    width : 144
    height: 144 // 70

    BorderImage {
        id: buttonImage
        source: "images/toolbutton.sci"
        width: propertyButton.width; height: propertyButton.height
    }

    CameraButton {
        anchors.fill: parent
        // previously selected popup image is used as the label instead of Text
        Image {
            anchors.centerIn: parent
            source: popup.currentItem.icon
        }
        // make the popup GridView list visible or invisible depending
        onClicked: popup.toggle()
    }

    // gridview list of all the filters available
    // the selected one is used as the current value and sent to the CameraButton onClicked
    CameraPropertyPopup {
        id: popup
        anchors.right: parent.left
        anchors.rightMargin: 16
        anchors.top: parent.top
        visible: opacity > 0

        currentValue: propertyButton.value

        onSelected: popup.toggle()
    }
}

