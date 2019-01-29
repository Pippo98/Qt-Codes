import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {

    header: Label {
        text: "Graphs"
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    Slider{
        id: pointsSlider1
        from: 100
        to: 1500
        value: 500
        onValueChanged: backend.sliderChanged(0, value)
        Text{
            text: "Points Number"
            color: "lightGray"
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Component.onCompleted: backend.sliderChanged(0, value)
    }

    Slider{
        id: frameRateSlider2
        from: 100
        to: 1500
        value: 500
        anchors.left: pointsSlider1.right
        onValueChanged:{
            chart1.resizeInterval = Number(value)
            console.log(chart1.resizeInterval)
        }
        Text{
            text: "Y Axis Resizing Timer"
            color: "lightGray"
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    GraphChart{
        id: chart1
        anchors.top: pointsSlider1.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }
}


















