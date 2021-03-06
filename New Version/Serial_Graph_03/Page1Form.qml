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
        text: "Control Panel"
        horizontalAlignment: Text.AlignHCenter
        //fontSizeMode: Text.FixedSize
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
}
