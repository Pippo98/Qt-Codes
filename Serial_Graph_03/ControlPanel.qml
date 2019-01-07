import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4

Item {
    id: item1
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
                interval: 1000
                onTriggered: {comboBox.members = serial.detectPort()}
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

            property variant items: ["Close", "Open"]
            property int currentSelection: 1
            property int identifier: 0

            anchors.left: parent.left
            anchors.leftMargin: 10
            text: items[currentSelection]
            onClicked: {
                currentSelection = (currentSelection + 1) % items.length
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
                switchColumn.visible = checked
            }
        }
    }
}
