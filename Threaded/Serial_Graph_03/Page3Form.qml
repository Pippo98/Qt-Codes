import QtQuick 2.9
import QtQuick.Controls 2.2

Page {
    width: 600
    height: 400

    header: Label {
        text: qsTr("Terminal")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    Connections{
        target: serial
        onDataChanged:{
            console.log("qml slot")
            textArea.text = data
        }
    }

    Timer{
        id: textTim
        interval: 5
        repeat: true
        running: true
        onTriggered: textArea.text = serial.getText()
    }

    Text {
        id: textArea
        //font.family: "Source Code Pro"
        color: "white"
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }
}
