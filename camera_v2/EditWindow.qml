import QtQuick 2.0

import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0

import CropBackend 1.0

// Secondary Window
Window {
    id: cropWindow
    signal signalExit   // Declare signal for exit but define below
    signal openEditInfo
    width: Screen.width // / 2
    height: Screen.height // Screen.width / 2
    property alias source: image.source
    property string path
    // property int rotateAngle: 0

    Crop {
        id: cropObj
        onClippedImage: {
            // image.performCrop()
            image.source = "file:///" + new_file
            image.path = new_file
            // console.log("image.path after crop: ", image.path)
            // image.selection.destroy()
            editInfo.show()
            cropWindow.hide()
        }
    }

    Rectangle{
        id: windowRect
        width: parent.width
        height: parent.height

        Rectangle {
            id: imageRect
            width: parent.width
            height: parent.width // parent.height - (parent.height / 4)
            color: "black"

            PhotoCrop {
                id: image
                // rotation: rotateAngle
            }
        }

        GridLayout {
            id: grid
            width: parent.width
            height: parent.height / 4
            columnSpacing: 10 // / 3
            rowSpacing: 7 // 5
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            rows: 3 // 4
            // columns: 2

            /*
            Button {
                id: rotateLeft
                text: qsTr("Rotate Left")
                Layout.fillWidth: true
                Layout.preferredHeight: grid.height / grid.rows - 10
                Layout.row: 0
                Layout.column: 0
                onClicked: {
                    image.rotation = -90
                    rotateAngle = -90
                    enabled = false
                }
            }

            Button {
                id: rotateRight
                text: qsTr("Rotate Right")
                Layout.fillWidth: true
                Layout.preferredHeight: grid.height / grid.rows - 10
                Layout.row: 0
                Layout.column: 1
                onClicked: {
                    image.rotation = 90
                    rotateAngle = 90
                    enabled = false
                }
            }
            */

            // Crop button
            Button {
                id: cropButton
                text: qsTr("Crop / Edit Info")
                Layout.fillWidth: true
                Layout.preferredHeight: grid.height / grid.rows - 10
                Layout.row: 0 // 1
                // Layout.columnSpan: 2
                // the signal is defined when it is used in the main window
                onClicked: {
                    if (image.selection)
                    {
                        // text1.text = image.selection.width  // .x, .y, .height
                        // use grabToImage and send this data back to C++.
                        cropObj.applyCrop(path, image.width, image.height, image.selection.width,
                                          image.selection.height, image.selection.x, image.selection.y)
                    }
                }
            }

            // Button to open the main application window
            Button {
                id: backButton
                text: qsTr("Back")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.row: 1 // 2
                Layout.fillWidth: true
                Layout.preferredHeight: grid.height / grid.rows - 10
                // Layout.columnSpan: 2
                // the signal is defined when it is used in the main window
                onClicked: {
                    // image.undo()
                    image.resetDims()
                    cropWindow.signalExit() // invoke signal
                    image.rotation = 0
                }
            }

            Text {
                id: text1
                Layout.row: 2 // 3
                Layout.fillWidth: true
                Layout.preferredHeight: grid.height / grid.rows - 20
                text: cropWindow.path // qsTr("Text")
                font.pixelSize: 20
                // Layout.columnSpan: 2
            }
        }
    }

    //--------------------------------------------------------------
    EditInfo {
        id: editInfo
        title: qsTr("Edit Info")
        source: image.source
        path: image.path
        // rotatedAngleEI: rotateAngle
        onSignalBack: {
            editInfo.close()
            image.resetDims()
            image.source = "file:///" + cropWindow.path
            // console.log('angle: ', cropWindow.rotateAngle)
            // rotatedAngleEI = 0
            cropWindow.show()
        }

        onNewPhoto: {
            editInfo.close()
            image.resetDims()
            /*
            cropWindow.rotateAngle = 0
            image.rotation = 0
            rotatedAngleEI = 0
            rotateRight.enabled = true
            rotateLeft.enabled = true
            */
            cropWindow.signalExit()
        }
    }
}

/*##^## Designer {
    D{i:4;anchors_height:86;anchors_x:686}
}
 ##^##*/
