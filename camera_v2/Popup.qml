import QtQuick 2.0

// Defnine what a Popup item is
// Basically brings up a button if clicked
// E.g le the an available cameras popup when this item is clicked
// to show a list of multiple cameras, ListView is defined and the values are selected

Rectangle {
    id: popup

    radius: 5
    border.color: "#000000"
    border.width: 2
    smooth: true
    color: "#5e5e5e"

    state: "invisible"

    // if list of cameras button not clicked, let the popup not be shown and viceversa
    states: [
        State {
            name: "invisible"
            PropertyChanges { target: popup; opacity: 0 }
        },

        State {
            name: "visible"
            PropertyChanges { target: popup; opacity: 1.0 }
        }
    ]

    transitions: Transition {
        NumberAnimation { properties: "opacity"; duration: 100 }
    }

    function toggle() {
        if (state == "visible")
            state = "invisible";
        else
            state = "visible";
    }
}
