import QtQuick 2.0
import QtMultimedia 5.4

// Buttons for Image Capture mode
// Focus scopes assist in keyboard focus handling when building reusable QML components.
FocusScope {
    property Camera camera
    property bool previewAvailable : false
    property bool cropAvailable : false

    property int buttonsPanelWidth: buttonPaneShadow.width

    signal previewSelected
    signal cropSelected
    // signal videoModeSelected

    id : captureControls

    // sets the buttons to the top right of the window
    Rectangle {
        id: buttonPaneShadow
        width: bottomColumn.width + 25 // 16
        height: parent.height
        anchors.top: parent.top
        anchors.right: parent.right
        color: Qt.rgba(0.08, 0.08, 0.08, 1)

        // Top column buttons
        Column {
            anchors {
                right: parent.right
                top: parent.top
                margins: 8
            }

            id: buttonsColumn
            spacing: 50 // 10

            FocusButton {
                camera: captureControls.camera
                // visible if camera is active and supports focus mode
                visible: camera.cameraStatus == Camera.ActiveStatus && camera.focus.isFocusModeSupported(Camera.FocusAuto)
            }

            // Capture button
            CameraButton {
                text: "Capture"
                visible: camera.imageCapture.ready
                onClicked: camera.imageCapture.capture()
            }

            // Camera Filters
            CameraPropertyButton {
                id : wbModesButton

                visible: false

                // sets the Auto mode as default
                value: CameraImageProcessing.WhiteBalanceAuto

                // creates model which contains data for the delegated CameraPropertyPopup items
                model: ListModel {
                    ListElement {
                        icon: "images/camera_auto_mode.png"
                        value: CameraImageProcessing.WhiteBalanceAuto
                        text: "Auto"
                    }
                    ListElement {
                        icon: "images/camera_white_balance_sunny.png"
                        value: CameraImageProcessing.WhiteBalanceSunlight
                        text: "Sunlight"
                    }
                    ListElement {
                        icon: "images/camera_white_balance_cloudy.png"
                        value: CameraImageProcessing.WhiteBalanceCloudy
                        text: "Cloudy"
                    }
                    ListElement {
                        icon: "images/camera_white_balance_incandescent.png"
                        value: CameraImageProcessing.WhiteBalanceTungsten
                        text: "Tungsten"
                    }
                    ListElement {
                        icon: "images/camera_white_balance_flourescent.png"
                        value: CameraImageProcessing.WhiteBalanceFluorescent
                        text: "Fluorescent"
                    }
                }
                onValueChanged: captureControls.camera.imageProcessing.whiteBalanceMode = wbModesButton.value
            }

            // To be visible only if Preview available
            // i.e. after an image has been captured
            CameraButton {
                text: "View"
                // if clicked, switch to the PhotoPreview cameraUI state
                onClicked: captureControls.previewSelected()

                // previewAvailable becomes true onImageCaptured in the Camera section
                // in main.qml
                // so the button becomes visible
                visible: captureControls.previewAvailable
            }
        }

        // Bottom column buttons
        Column {
            anchors {
                bottom: parent.bottom
                right: parent.right
                margins: 8
            }

            id: bottomColumn
            spacing: 50 // 10

            // created model data for the CameraListPopup ListView delegated items
            CameraListButton {
                model: QtMultimedia.availableCameras
                onValueChanged: captureControls.camera.deviceId = value
            }

            // To be visible only if Preview available
            // i.e. after an image has been captured
            CameraButton {
                text: "Crop"
                // switch to the cameraUI state  to EditWindow if clicked
                onClicked: captureControls.cropSelected()

                // so the button becomes visible
                visible: captureControls.cropAvailable
            }

            CameraButton {
                id: quitButton
                text: "Quit"
                onClicked: Qt.quit()
            }
        }
    }


    ZoomControl {
        x : 0
        y : 0
        width : 200
        height: parent.height

        currentZoom: camera.digitalZoom
        maximumZoom: Math.min(4.0, camera.maximumDigitalZoom)
        onZoomTo: camera.setDigitalZoom(value)
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
