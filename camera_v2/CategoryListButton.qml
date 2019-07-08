import QtQuick 2.0
import QtQuick.Window 2.2

import QtMultimedia 5.4

import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import Qt.labs.settings 1.0

// Definition of the Camera List button.
// Used basically to show the different available cameras

Item {
    id: categoryListButton

    // let the current value and model be those selected from the CameraListPopup function
    property alias value : popup.currentValue
    property alias displayText : popup.currentText
    property alias model : popup.model

    width : 144
    height: 70
    visible: model.length > 0

    BorderImage {
        id: buttonImage
        source: "images/toolbutton.sci"
        width: categoryListButton.width; height: categoryListButton.height
    }

    // the button that shows open the list (CameraListPopup)
    // of available cameras when clicked
    CameraButton {
        anchors.fill: parent
        // set the currently selected item to be displayed as the name
        // e.g. "Front-camera" if it is selected previously from the ListPopup,
        // "Rear-camera" if selected previously
        // Else if nothing selected before by the ListPopup (i.e. null, don't display anything)
        text: popup.currentItem != null ? popup.currentItem.displayName : "Category List"

        // if the button is clicked, switch the values from the popup to it.
        onClicked: popup.toggle()
    }

    CategoryListPopup {
        id: popup
        anchors.right: parent.left
        anchors.rightMargin: 16
        // anchors.top: parent.top
        anchors.bottom: parent.bottom
        visible: opacity > 0

        // show the popup if it is clicked
        onSelected: popup.toggle()
    }
}
