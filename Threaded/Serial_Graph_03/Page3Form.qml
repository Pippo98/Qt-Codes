import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {
    header: Label {
        text: qsTr("Send Page")
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    RowLayout{
        id: row
        anchors.fill: parent
        spacing: 10

        Rectangle{
            id: inputField

            property string propColor: "lightgrey"

            Layout.alignment: Qt.AlignTop
            width: parent.width
            height: width/ 20

            radius: height / 2
            color: propColor

            TextInput{
                id: input

                anchors.fill: inputField
                anchors.leftMargin: 20
                anchors.topMargin: 5

                font.pixelSize: 20

                text: "192 0 1 2 3 4 5 6 7"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                onEditingFinished: {
                    serial.sendCommand(text)
                }
                Component.onCompleted: {
                    serial.initTextField(inputField)
                }
            }
        }

        TextArea{
            id: textBox
            Layout.fillWidth: true

            anchors.top: inputField.bottom
            anchors.horizontalCenter: inputField.horizontalCenter
            anchors.bottom: parent.bottom

            readOnly: false

            property string line1: ""
            property string line2: ""
            property string line3: ""
            property string line4: ""
            property string line5: ""
            property string line6: ""

            Component.onCompleted: {
                serial.initTextArea(textBox)
            }

            text: line1+line2+line3+line4+line5+line6
        }

        function setQmlText(txt){
            textBox.displayableText = txt
        }

    }
}
