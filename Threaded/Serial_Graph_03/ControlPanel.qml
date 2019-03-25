import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4

Item {
    id: item1

    Connections{
        target: backend
        onPortStateChanged:{
            button.currentSelection = state
            console.log("onPortStateChanged: " + state)
        }
    }

    Column {
        id: column
        ComboBox {
            id: comboBox

            property variant members
            property int currentSelection: 0
            property int identfier: 0

            model: members
            anchors.left: parent.left
            anchors.leftMargin: 10
            onActivated: {
                backend.comboChanged(currentText, currentIndex, identfier)
                textArea.text = serial.portInfo(currentText)
            }
            Timer{
                running: true
                repeat: true
                interval: 500
                onTriggered: {
                    comboBox.members = serial.detectPort()
                }
            }
        }

        ComboBox{
            id: comboBox1

            property variant members
            property int currentSelection: 0
            property int identfier: 1

            model: ["", "9600", "115200", "225000", "2225000"]
            anchors.left: parent.left
            anchors.leftMargin: 10
            currentIndex: 4
            onActivated: {
                backend.comboChanged(currentText, currentIndex, identfier)
            }
            Component.onCompleted: {
                backend.comboChanged(currentText, currentIndex, identfier)
            }
        }

        TextArea {
            id: textArea
            height: 100
            width: parent.width
            clip: false
            enabled: false
            anchors.left: parent.left
            anchors.leftMargin: 10
            wrapMode: TextEdit.Wrap
        }

        Button {
            id: button

            property variant items: ["Closed", "Opened"]
            property string red: "<font color='red'>"
            property string green: "<font color='green'>"
            property int currentSelection: 0
            property int identifier: 0

            anchors.left: parent.left
            anchors.leftMargin: 10
            text: {
                if(currentSelection){
                    green + items[currentSelection]
                }
                else{
                    red + items[currentSelection]
                }
            }
            onClicked: {
                backend.buttonClicked(identifier, currentSelection)
            }
        }

        Switch{
            id: logSwitch
            checked: false
            text: "Log Enable"
            onCheckedChanged: {
                backend.logSwitchChanged(checked);
            }
        }

        Switch{
            id: canModeSwitch
            checked: true
            text: "CAN Mode"
            onCheckedChanged: {
                serial.setCanMode(checked)
                switchPanel.visible = checked
            }
        }
        Button{
            id: infoButton

            property int identifier: 2
            property int currentSelection: 0

            text: "Help"
            onClicked: {
                backend.buttonClicked(identifier, currentSelection)
            }
        }
    }
}
