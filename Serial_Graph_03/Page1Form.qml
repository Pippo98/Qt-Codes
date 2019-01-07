import QtQuick 2.11
import QtCharts 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4

Page {
    id: page
    width: 600
    height: 400

    header: Label {
        id: header
        text: qsTr("Control Panel")
        horizontalAlignment: Text.AlignHCenter
        //fontSizeMode: Text.FixedSize
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    Connections{
        target: serial
        onGraphsChanged:{
            switchColumn.members = number
        }
    }

    ControlPanel{
        id: controlPanel
        width: 170
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.topMargin: 6
        anchors.left: parent.left
    }


    ColumnLayout{
        id: switchColumn
        anchors.left: controlPanel.right
        anchors.top : parent.top
        property int members: 10
        spacing: 1

        Switch{
            text: "Frontal"
            checked: true
            onCheckedChanged: {
                backend.switchChanged(0, checked)
            }
            Component.onCompleted: {
                backend.switchChanged(100, 1)
            }
        }
        Switch{
            text: "Pedals"
            checked: true
            onCheckedChanged: {
                backend.switchChanged(1, checked)
            }
        }
        Switch{
            text: "ECU"
            checked: true
            onCheckedChanged: {
                backend.switchChanged(2, checked)
            }
        }
        Switch{
            text: "Inverter"
            checked: true
            onCheckedChanged: {
                backend.switchChanged(3, checked)
            }
        }
        Switch{
            text: "Low Voltage"
            checked: true
            onCheckedChanged: {
                backend.switchChanged(4, checked)
            }
        }
        Switch{
            text: "High Voltage"
            checked: true
            onCheckedChanged: {
                backend.switchChanged(5, checked)
            }
        }
    }
}
