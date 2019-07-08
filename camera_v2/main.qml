import QtQuick 2.0
import QtQuick.Window 2.2

import QtMultimedia 5.4

import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import Qt.labs.settings 1.0

ApplicationWindow {
    id: mainWindow
    visible: true
    width: Screen.width // / 2
    height: Screen.height // / 2
    // title: qsTr("Simple Camera")

    // Camera is set as the main page
    Rectangle {
        id : cameraUI
        width: parent.width
        height: parent.height

        color: "black"

        // initially on image photoraph state
        state: "PhotoCapture"

        // defines what the different states should activate
        states: [
            State {
                name: "PhotoCapture"
                StateChangeScript {
                    script: {
                        camera.captureMode = Camera.CaptureStillImage
                        camera.start()
                    }
                }
            },

            State {
                name: "PhotoPreview"
            },

            State {
                name: "EditWindow"
                StateChangeScript {
                    script: {
                        camera.stop()
                        mainWindow.hide()
                        editWindow.show()
                    }
                }
            }/*,

            State {
                name: "EditInfo"
                StateChangeScript {
                    script: {
                        editInfo.path = editWindow.path
                        editInfo.source = editWindow.source  // cropWindow.source
                        editWindow.hide()
                        editInfo.show()
                    }
                }
            }
            */
        ]

        Camera {
            id: camera

            // since cameraUI state initially on "PhotoCapture", set the camera to images
            captureMode: Camera.CaptureStillImage

            // after image is taken, show preview on the PhotoPreview defined image item
            imageCapture {
                onImageCaptured: {
                    photoPreview.source = preview
                    editWindow.source = preview
                    // editWindow.viewFinderWidth = viewfinder.width
                    // editWindow.viewFinderHeight = viewfinder.height

                    // during the preview mode, let the camera buttons still be active
                    stillControls.previewAvailable = true
                    stillControls.cropAvailable = true

                    // after the image is captured, switch to preview mode
                    cameraUI.state = "PhotoPreview"
                }
                onImageSaved: editWindow.path = path
                            // editWindow.source = "file:///" + path
            }

            videoRecorder {
                 resolution: "640x480"
                 frameRate: 30
            }
        }

        // call the PhotoPreview function and define the signals    in it
        PhotoPreview {
            id : photoPreview
            anchors.fill : parent

            // if it's closed, let the photocapture mode be activated
            onClosed: cameraUI.state = "PhotoCapture"
            // activate when the camera state has been switched to preview (i.e. after an
            // image or video has been taken. See above
            visible: cameraUI.state == "PhotoPreview"
            focus: visible
        }

        // define the viewfinder that actually shows the camera in any mode
        VideoOutput {
            id: viewfinder
            // it is visible once the camera is in capture mode regardless
            // of if it's capturing an image or video
            visible: cameraUI.state == "PhotoCapture" // || cameraUI.state == "VideoCapture"

            x: 0
            y: 0

            // width is taken as the remaining part of the screen after the
            // controller buttons have been defined
            width: parent.width - stillControls.buttonsPanelWidth
            height: parent.height

            source: camera
            autoOrientation: true
        }

        PhotoCaptureControls {
            id: stillControls
            anchors.fill: parent
            camera: camera

            // visible when the camera is in capture state
            visible: cameraUI.state == "PhotoCapture"
            // state after an image has been taken
            onPreviewSelected: cameraUI.state = "PhotoPreview"
            onCropSelected: cameraUI.state = "EditWindow"
        }

        EditWindow {
            id: editWindow
            title: qsTr("Edit Window")

            // The signal handler for the opening of the main window
            onSignalExit: {
                editWindow.close()     // Close the first window
                mainWindow.show()       // Shows the main window
                cameraUI.state = "PhotoCapture"
            }
            onOpenEditInfo: cameraUI.state = "EditInfo"
        }
        /*
        EditInfo {
            id: editInfo
            title: qsTr("Edit Info")

            onSignalBack: {
                editInfo.close()
                // editWindow.show()
                cameraUI.state = "EditWindow"
            }
            onNewPhoto: {
                editInfo.close()
                editWindow.signalExit()
                // cameraUI.state = "PhotoCapture"
            }
        }
        */
    }
}
