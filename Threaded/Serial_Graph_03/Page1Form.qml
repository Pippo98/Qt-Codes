import QtQuick 2.0
import QtCharts 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

Page {
    id: page
    width: 600
    height: 400

    header: Label {
        id: header
        text: "Control Panel"
        horizontalAlignment: Text.AlignHCenters
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    ControlPanel{
        id: controlPanel
        width: 170
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.topMargin: 6
        anchors.left: parent.left
    }

    SwitchPanel{
        id: switchPanel
        anchors.left: controlPanel.right
        anchors.top : parent.top
    }

    TextArea{
        id: infoArea
        anchors.right: parent.right
        anchors.top: parent.top

        property string displayableText

        text: displayableText

        Component.onCompleted: {
            serial.initInfoTextArea(infoArea)
        }
    }
}
