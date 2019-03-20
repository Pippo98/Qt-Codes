import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4

Item {
    RowLayout{
        id: rowLayout/*
        spacing: 2
        anchors.fill: parent
        property variant switchSelections: []*/
        ColumnLayout{
            id: switchColumn
            Layout.alignment: Qt.AlignLeft
            Layout.fillWidth: true

            property variant switchNames: serial.getSwitchNames();

            Repeater{
                model: switchColumn.switchNames.length
                Switch{
                    id: sw1
                    text: switchColumn.switchNames[index]
                    checked: false
                    onCheckedChanged: {
                        serial.setPrimarySwitches(index, checked)
                        //rowLayout.switchSelections[index] = checked
                        //console.log(rowLayout.switchSelections)
                    }
                    onHoveredChanged: {
                        if(hovered){
                            secondarySwitch.parentIindex = index
                        }
                    }/*
                    Component.onCompleted: {
                        rowLayout.switchSelections.push(false)
                    }*/
                }
            }
        }/*
        Repeater{
            model: switchColumn.switchNames.length
            SecondarySwitch_2{
                id: secondarySwitch
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                parentIindex: index
                isVisible: rowLayout.switchSelections[index] | false
            }
        }*/
        SecondarySwitch{
            id: secondarySwitch
            anchors.left: parent.right
        }
    }
}
