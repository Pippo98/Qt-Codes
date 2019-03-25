import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4

Item {
    id: secondarySwitch

    property int parentIindex
    property int switchesNumber
    property variant switchesNames: serial.getSecondarySwitchNames(parentIindex)
    property variant selections: serial.getSecondarySwitchesSelections(parentIindex)

    ColumnLayout{
        visible: true
        Repeater{
            model: switchesNames.length
            Switch{
                id: sw
                text:
                    switchesNames[index]
                checked: selections[index] | false
                onCheckedChanged: {
                    serial.setSecondarySwitchesSelections(parentIindex, index, checked)
                }
            }
        }
    }
    onParentIindexChanged: {
        selections = serial.getSecondarySwitchesSelections(parentIindex)
        switchesNames = serial.getSecondarySwitchNames(parentIindex)
    }
}
