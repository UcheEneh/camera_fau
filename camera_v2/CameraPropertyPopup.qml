import QtQuick 2.0

// Similar to CameraListPopup, just now for displaying the various filters that can
// be added to the camera before taking picture

Popup {
    id: propertyPopup

    // model - contains the data and its structure.
    // This is taken from the item selected that is delegatedin ListView
    property alias model : view.model
    property variant currentValue
    property variant currentItem : model.get(view.currentIndex)

    property int itemWidth : 100
    property int itemHeight : 144// 70

    // two columns because more items in the list / grid this time
    property int columns : 2

    width: columns*itemWidth + view.anchors.margins*2
    height: Math.ceil(model.count/columns)*itemHeight + view.anchors.margins*2 + 25

    signal selected

    // since we are dealing with multiplel columns
    function indexForValue(value) {
        for (var i = 0; i < view.count; i++) {
            if (model.get(i).value == value) {
                return i;
            }
        }

        return 0;
    }

    GridView {
        id: view
        anchors.fill: parent
        anchors.margins: 5
        cellWidth: propertyPopup.itemWidth
        cellHeight: propertyPopup.itemHeight
        snapMode: ListView.SnapOneItem
        highlightFollowsCurrentItem: true
        highlight: Rectangle { color: "gray"; radius: 5 }
        currentIndex: indexForValue(propertyPopup.currentValue)

        // delegate items to the GridView using the values if they are clicked on to select
        // the desired property
        delegate: Item {
            width: propertyPopup.itemWidth
            height: 70

            // Instead of a Text as in CameraListPopup, use an image
            Image {
                anchors.centerIn: parent
                source: icon
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    // use value of item to set the property variables in propertyPopup
                    propertyPopup.currentValue = value
                    propertyPopup.selected(value)
                }
            }
        }
    }

    // Text to indicate what has been selected
    Text {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 16

        color: "#ffffff"
        font.bold: true
        style: Text.Raised;
        styleColor: "black"
        font.pixelSize: 14

        // set the text as what was selected from the GridView
        text: view.model.get(view.currentIndex).text
    }
}
