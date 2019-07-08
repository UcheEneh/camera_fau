import QtQuick 2.0

// Definition of the Camera List button.
// Used basically to show the different available cameras

Item {
    id: cameraListButton

    // let the current value and model be those selected from the CameraListPopup function
    property alias value : popup.currentValue
    property alias model : popup.model

    width : 144
    height: 144 // 70
    visible: model.length > 0

    BorderImage {
        id: buttonImage
        source: "images/toolbutton.sci"
        width: cameraListButton.width; height: cameraListButton.height
    }

    // the button that shows open the list (CameraListPopup)
    // of available cameras when clicked
    CameraButton {
        anchors.fill: parent
        // set the currently selected item to be displayed as the name
        // e.g. "Front-camera" if it is selected previously from the ListPopup,
        // "Rear-camera" if selected previously
        // Else if nothing selected before by the ListPopup (i.e. null, don't display anything)
        text: popup.currentItem != null ? popup.currentItem.displayName : ""

        // if the button is clicked, switch the values from the popup to it.
        onClicked: popup.toggle()
    }

    CameraListPopup {
        id: popup
        anchors.right: parent.left
        anchors.rightMargin: 16
        anchors.top: parent.top
        visible: opacity > 0

        // show the popup if it is clicked
        onSelected: popup.toggle()
    }
}
