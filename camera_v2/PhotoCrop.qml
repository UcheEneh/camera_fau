
import QtQuick 2.0
import QtQuick.Window 2.2

import QtMultimedia 5.4

import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import Qt.labs.settings 1.0

import QtGraphicalEffects 1.0

import CropBackend 1.0

// Rectangle
Item
{
    // use the 'preview' image defined in the calling function as the image to be displayed
    // Note: if using this directly in Edit Window, then the source doesn't need to be defined here
    property alias source : cropImage.source
    property var selection: undefined
    // property alias rotated : cropImage.rotated
    property string path

    id: actImageRect
    signal performCrop
    signal resetDims
    // signal rotate(var angle)
    width: parent.width
    height: parent.height

    onPerformCrop: {
        actImageRect.x = selection.x
        actImageRect.y = selection.y
        actImageRect.width = selection.width // windowRect.width - 400
        actImageRect.height = selection.height // windowRect.width - 400
    }
    onResetDims: {
        // cropImage.fillMode = Image.Stretch
        // cropImage.smooth = false
        actImageRect.width = parent.width
        actImageRect.height = parent.height
        actImageRect.x = parent.x
        actImageRect.y = parent.y
    }

    /*
    onRotate: {
        cropImage.rotation = angle
    }
    */

    Image {
        id: cropImage
        anchors.fill : parent
        // fillMode: Image.PreserveAspectFit
        // smooth: true
        // visible: false

        MouseArea {
            anchors.fill: parent
            onClicked: {
                // create the bounding box rectangle on the image
                if (!selection)
                    selection = selectionComponent.createObject(parent,
                                            {"x": parent.width / 4, "y": parent.height / 4,
                                            "width": parent.width / 4, "height": parent.width / 4})
            }
        }
    }

    // The Component type essentially allows QML components to be defined inline, within
    // a QML document, rather than as a separate QML file
    Component {
        id: selectionComponent

        // define bounding box
        Rectangle {
            id: selComp
            border {
                width: 2
                color: "steelblue"
            }
            // anchors.centerIn: parent.Center  // not working
            color: "#354682B4"
            property int rulersSize: 50

            // drag mouse area
            MouseArea {
                anchors.fill: parent
                // drag the whole box when moved and define boundary so it
                // isn't dragged out of the image region
                drag {
                    target: parent
                    minimumX: 0
                    minimumY: 0
                    // main window width - this rectangle width
                    maximumX: parent.parent.width - parent.width
                    maximumY: parent.parent.height - parent.height
                    smoothed: true
                }

                // destroy compinent
                onDoubleClicked: parent.destroy()
            }

            // Define the points for dragging

            // X-Axis:
            // CENTER LEFT
            Rectangle {
                width: rulersSize
                height: rulersSize
                radius: rulersSize
                color: "steelblue"
                anchors.horizontalCenter: parent.left
                anchors.verticalCenter: parent.verticalCenter

                MouseArea {
                    anchors.fill: parent
                    drag { target: parent; axis: Drag.XAxis}
                    onMouseXChanged: {
                        if (drag.active){
                            selComp.width = selComp.width - mouseX
                            selComp.x = selComp.x + mouseX

                            // prevent from completely dragging to the smalles
                            if(selComp.width < 30)
                                selComp.width = 30
                        }
                    }
                }
            }

            // CENTER RIGHT
            Rectangle {
                width: rulersSize
                height: rulersSize
                radius: rulersSize
                color: "steelblue"
                anchors.horizontalCenter: parent.right
                anchors.verticalCenter: parent.verticalCenter

                MouseArea {
                    anchors.fill: parent
                    drag{ target: parent; axis: Drag.XAxis }
                    onMouseXChanged: {
                        if(drag.active){
                            selComp.width = selComp.width + mouseX
                            if(selComp.width < 50)
                                selComp.width = 50
                        }
                    }
                }
            }

            // Y-Axis
            // CENTER TOP
            Rectangle {
                width: rulersSize
                height: rulersSize
                radius: rulersSize
                x: parent.x / 2
                y: 0
                color: "steelblue"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.top

                MouseArea {
                    anchors.fill: parent
                    drag{ target: parent; axis: Drag.YAxis }
                    onMouseYChanged: {
                        if(drag.active){
                            selComp.height = selComp.height - mouseY
                            selComp.y = selComp.y + mouseY
                            if(selComp.height < 50)
                                selComp.height = 50
                        }
                    }
                }
            }

            // CENTER BOTTOM
            Rectangle {
                width: rulersSize
                height: rulersSize
                radius: rulersSize
                x: parent.x / 2
                y: parent.y
                color: "steelblue"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.bottom

                MouseArea {
                    anchors.fill: parent
                    drag{ target: parent; axis: Drag.YAxis }
                    onMouseYChanged: {
                        if(drag.active){
                            selComp.height = selComp.height + mouseY
                            if(selComp.height < 50)
                                selComp.height = 50
                        }
                    }
                }
            }
        }
    }
}

