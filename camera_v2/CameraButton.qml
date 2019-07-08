import QtQuick 2.0

Item {
    id: button

    signal clicked

    property string text
    property color color: "white"

    width : 144
    height: 144 // 70

    BorderImage {
        id: buttonImage
        source: "images/toolbutton.sci"
        width: button.width; height: button.height
    }
    MouseArea {
        id: mouseRegion
        anchors.fill: buttonImage

        // once any region on this item is clicked, activate the btton.clicked signal
        // that is to be defined in the calling function
        onClicked: { button.clicked(); }
    }
    Text {
        id: btnText
        anchors.fill: buttonImage
        anchors.margins: 5
        text: button.text
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        color: button.color
        font.bold: true
        style: Text.Raised
        styleColor: "black"
        font.pixelSize: 20 // 14
    }
}
