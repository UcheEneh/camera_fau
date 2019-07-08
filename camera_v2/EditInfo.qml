import QtQuick 2.0
import QtQuick.Window 2.2

import QtMultimedia 5.4

import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import Qt.labs.settings 1.0

import ToDBBackend 1.0

// c++ properties would be included for this

Window {
    id: editInfo
    // color: "white"
    signal closed
    signal signalBack
    signal newPhoto

    width: Screen.width // / 2
    height: Screen.height // Screen.width / 2

    property alias source: image.source
    property string path
    // property int rotatedAngleEI: 0

    property alias name: name
    property alias category: category
    property alias date: date
    property alias comment: comment
    property alias saveButton: saveButton

    ToDB {
        id: toDB
        onUpdateFinished: statusLabel.text = updateMsg // console.log("Info Updated")
        onNetworkError: statusLabel.text = errorMsg // console.log("Network Error", e)
    }

    Rectangle {
        id: rectangle
        x: 0
        y: 0
        width: parent.width
        height: parent.height // Screen.height //480

        Rectangle {
            // to display the image
            id: rect1
            width: parent.width
            height: parent.width // parent.height - (parent.height / 3) - (100)
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.left: parent.left
            color: "black"

            Image {
                id: image
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                smooth: true
                // rotation: rotatedAngleEI
                // source: "file:///" + editInfo.path
            }
        }

        GridLayout {
            id: infoGrid
            width: parent.width
            height: parent.height - parent.width - 100 // parent.height / 3
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            clip: false
            anchors.topMargin: 0
            // anchors.top: rect1.bottom
            anchors.bottom: buttonsGrid.top
            // anchors.centerIn: parent.Center
            rows: 5
            columns: 3


            // CATEGORIES
            // Name
            Label {
                id: nameLabel
                text: "Name"
                Layout.alignment: Qt.AlignLeft
            }

            TextField {
                id: name
                Layout.alignment: Qt.AlignLeft // Qt.AlignHCenter | Qt.AlignVCenter
                Layout.preferredWidth: infoGrid.width - name.x - 15
                Layout.columnSpan: 2
            }

            // Category
            Label {
                id: categoryLabel
                Layout.alignment: Qt.AlignLeft
                text: "Category"
                Layout.row: 1
                Layout.column: 0

            }

            TextField {
                id: category
                Layout.alignment: Qt.AlignLeft // Qt.AlignHCenter | Qt.AlignVCenter
                Layout.row: 1
                Layout.column: 1
                Layout.preferredWidth: infoGrid.width - category.x - 200
            }

            CategoryListButton {
                id: categoryListButton
                x: category.x + category.width + 10
                y: category.y
                // opacity: 1
                visible: true
                model: CategoryModel {}
                onValueChanged: {
                    if(value === "Others"){
                        category.text = ""
                        category.placeholderText = "Insert Catgory"
                    }
                    else
                        category.text = value // displayText
                }
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.row: 1
                Layout.column: 2
            }

            // Date
            Label {
                id: dateLabel
                text: "Date"
                Layout.alignment: Qt.AlignLeft
                Layout.row: 2
                Layout.column: 0
            }

            TextField {
                id: date
                topPadding: 5
                Layout.alignment: Qt.AlignLeft // Qt.AlignHCenter | Qt.AlignVCenter
                Layout.row: 2
                Layout.column: 1
                Layout.columnSpan: 2
                Layout.preferredWidth: infoGrid.width - date.x - 15
            }

            // Comment box
            Label {
                id: commentLabel
                text: "Comment"
                Layout.alignment: Qt.AlignLeft
                Layout.row: 3
                Layout.column: 0
            }

            TextField {
                id: comment
                Layout.alignment: Qt.AlignLeft // Qt.AlignHCenter | Qt.AlignVCenter
                Layout.row: 3
                Layout.column: 1
                Layout.columnSpan: 2
                Layout.preferredWidth: infoGrid.width - comment.x - 15 // width: (infoGrid.width / 3) * 2
            }

            // Status Label
            TextField {
                id: statusLabel
                placeholderText: qsTr("Status")
                // text: path
                font.pointSize: 9
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter // Qt.AlignLeft
                Layout.row: 4
                Layout.column: 0
                Layout.columnSpan: 3
                Layout.preferredWidth: infoGrid.width
            }
        }

        GridLayout {
            id: buttonsGrid
            width: parent.width
            height: 100 // (parent.height / 4) - 100
            // y: 8
            rowSpacing: 2 // 5
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.topMargin: 5
            rows: 1
            columns: 3
            columnSpacing: 5

            // Save button
            Button {
                id: saveButton
                text: "Save"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.row: 0
                Layout.column: 0
                Layout.fillWidth: true
                Layout.preferredWidth: buttonsGrid.width / 3
                Layout.preferredHeight: buttonsGrid.height / buttonsGrid.rows - 2

                onClicked: {
                    // statusLabel.text = path
                    function getTextLength()
                    {
                        var nameLength = name.length
                        var categoryLength = category.length
                        if(nameLength == 0 || categoryLength == 0)
                            return 0
                        return 1
                    }
                    var nameLen = getTextLength()
                    nameLen > 0 ? toDB.sendToDB(path, name.text, category.text, date.text, comment.text) :
                                            statusLabel.text = "Please insert information before Saving"

                    // nameLen > 0 ? statusLabel.text = nameLen : statusLabel.text = "Please insert information before Saving"
                    console.log("image.source after crop: ", image.source)
                }
            }

            Button {
                id: backButton
                text: "Undo Crop"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.row: 0
                Layout.column: 1
                Layout.fillWidth: true
                Layout.preferredWidth: buttonsGrid.width / 3
                Layout.preferredHeight: buttonsGrid.height / buttonsGrid.rows - 2
                onClicked: editInfo.signalBack()
            }

            Button {
                id: exitButton
                text: "New Photo"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.row: 0
                Layout.column: 2
                Layout.fillWidth: true
                Layout.preferredWidth: buttonsGrid.width / 3
                Layout.preferredHeight: buttonsGrid.height / buttonsGrid.rows - 2
                onClicked: {
                    editInfo.newPhoto()
                    // delete whatever is contained in the textfield
                    name.remove(0, name.length)
                    category.remove(0, category.length)
                    date.remove(0, date.length)
                    comment.remove(0, comment.length)

                    // image.rotation = 0
                }
            }
        }

    }

    // Current Date
    QtObject {
        property var locale: Qt.locale()
        property date currentDate: new Date()
        // property string dateString

        // Component.onCompleted fn: once this Component (here, QtObject) has been created, do this
        Component.onCompleted: {
            // Maybe not good idea to put current date as the default. May confuse person
            date.text = Qt.formatDateTime(currentDate, "dd.MM.yyyy")
        }
    }

    // if this is a Dialog, and Ok is clicked, the signal finished is activated
    // onAccepted: finished(form.name.text, form.category.text, form.date.text, form.comment.text)
}

/*##^## Designer {
    D{i:3;anchors_y:0}D{i:5;anchors_height:220}D{i:17;anchors_x:257}
}
 ##^##*/
