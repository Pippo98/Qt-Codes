import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

Item {
    RowLayout{
        id: rowLayout
        ColumnLayout{
            id: switchColumn
            Layout.alignment: Qt.AlignLeft
            Layout.fillWidth: true

            property variant switchNames: serial.getSwitchNames();

            Repeater{
                model: switchColumn.switchNames.length
                Switch{
                    id: sw1
                    text: "<font color='#ed1536'>" + switchColumn.switchNames[index]
                    checked: false
                    onCheckedChanged: {
                        serial.setPrimarySwitches(index, checked)
                    }
                    onHoveredChanged: {
                        if(hovered){
                            secondarySwitch.parentIindex = index
                        }
                    }
                }
            }
        }
        SecondarySwitch{
            id: secondarySwitch
            anchors.left: parent.right
        }
    }
}
