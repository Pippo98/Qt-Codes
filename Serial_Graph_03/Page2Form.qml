import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {

    header: Label {
        text: "Graphs"
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    GraphChart{
        id: chart1
    }
}


















