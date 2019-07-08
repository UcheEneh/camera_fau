import QtQuick 2.0

// create a list of models (in the used cases, camera devices) and activates on selection
Popup {
    id: categoryListPopup

    property alias model : view.model
    property variant currentValue
    property variant currentText

    // Let the current item displayed be the one selected in the ListView list
    property variant currentItem : model[view.currentIndex]

    property int itemWidth : 200
    property int itemHeight : 50

    // expand the width using the size of the items in the listview
    width: itemWidth + view.anchors.margins*2

    // height: view.count:- use the total count of items found and placed in the ListView
    height: view.count * itemHeight + view.anchors.margins*2

    // basically used to make the ListPopup visible or invisible
    signal selected

    ListView {
        id: view
        anchors.fill: parent
        anchors.margins: 5
        snapMode: ListView.SnapOneItem      // onyly select one item at a time
        highlightFollowsCurrentItem: true   // only currentl selected item is highlighted
        highlight: Rectangle { color: "gray"; radius: 5 }
        currentIndex: 0     // selection should start from first item

        // Creates different items and puts into the list (with the index indicating its position
        // E.g. delegates two items if device has two cameras

        // model: CategoryModel {}
        delegate: Item {
            width: categoryListPopup.itemWidth
            height: categoryListPopup.itemHeight

            Text {
                id: thisText
                // name is whatever the display name from the modelData
                text: model.displayName // modelData.displayName

                anchors.fill: parent
                anchors.margins: 5
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                color: "white"
                font.bold: true
                style: Text.Raised
                styleColor: "black"
                font.pixelSize: 14
            }
            MouseArea {
                anchors.fill: parent

                // if the a dlegated item is clicked, it is highlighed and selected
                onClicked: {
                    view.currentIndex = index
                    // sets the value for the cameraList Popup, and is selected
                    categoryListPopup.currentValue = model.deviceId
                    categoryListPopup.currentText =  model.displayName // thisText.text
                    categoryListPopup.selected(model.deviceId)
                }
            }
        }
    }
}
