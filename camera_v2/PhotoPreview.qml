import QtQuick 2.0
import QtMultimedia 5.0

Item {
    // use the 'preview' image defined in the calling function as the image to be displayed
    property alias source : preview.source
    signal closed

    Image {
        id: preview
        anchors.fill : parent
        fillMode: Image.PreserveAspectFit
        smooth: true
    }

    MouseArea {
        anchors.fill: parent
        // close item once the image is clicked
        onClicked: {
            parent.closed();
        }
    }
}

