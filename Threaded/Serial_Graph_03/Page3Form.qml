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

    ColumnLayout{
        id: row
        //anchors.fill: parent
        spacing: 1
        RowLayout{
            id: sendRow
            Layout.fillHeight: true
            Layout.fillWidth: true

            Layout.preferredWidth: row.width
            Layout.preferredHeight: width / 30
            Rectangle{
                id: inputField

                property string propColor: "lightgrey"

                Layout.alignment: Qt.AlignCenter
                Layout.fillHeight: true
                Layout.fillWidth: true

                Layout.maximumWidth: sendRow.width / 12 * 6
                Layout.maximumHeight: sendRow.height

//                width: parent.width / 12 * 6
//                height: width / 10

                radius: height / 8
                color: propColor


                TextInput{
                    id: input

                    anchors.fill: parent

                    font.pixelSize: 20

                    text: "192 0 1 2 3 4 5 6 7"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    Component.onCompleted: {
                        serial.initTextField(inputField)
                    }
                }
            }
            Rectangle{
                id: timeField

                property string propColor: "lightgrey"

                Layout.alignment: Qt.AlignCenter
                Layout.fillHeight: true
                Layout.fillWidth: true

                Layout.maximumWidth: parent.width / 12 * 3
                Layout.maximumHeight: inputField.height

                radius: height / 8
                color: propColor

                TextInput{
                    id: timeDelay

                    anchors.fill: timeField

                    font.pixelSize: 20

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                }
            }

            Button{
                id: sendButton

                Layout.alignment: Qt.AlignCenter
                Layout.fillHeight: true
                Layout.fillWidth: true

                Layout.maximumWidth: parent.width / 12 * 3
                implicitHeight: inputField.height * 4

                //implicitWidth: parent.width / 12 * 3

                text: "Send"

                onClicked: {
                    serial.sendCommand(input.text, timeDelay.text)
                }
            }
        }

        Text{
            id: title

            Layout.alignment: Qt.AlignCenter

            color: "white"
            font.pointSize: 20
            text: "CAN Sniffer"
        }

        TextArea{
            id: textBox
            Layout.fillWidth: true

//            anchors.top: title.bottom
//            anchors.horizontalCenter: title.horizontalCenter
//            anchors.bottom: parent.bottom

            readOnly: false

            property string displaybleText: ""

            Component.onCompleted: {
                serial.initTextArea(textBox)
            }

            //text: line1+line2+line3+line4+line5+line6
            font.pointSize: 10
            color: "#10e5d7"
            text: "timestamp\tid\tpay0\tpay1\tpay2\tpay3\tpay4\tpay5\tpay6\tpay7\r\n"+displaybleText
        }

        function setQmlText(txt){
            textBox.displayableText = txt
        }

    }
}



//            TextInput{
//                id: input

//                anchors.fill: inputField
//                anchors.leftMargin: 20
//                anchors.topMargin: 5

//                horizontalAlignment: Text.AlignHCenter
//                verticalAlignment: Text.AlignVCenter

//                maximumLength: 3

//                inputMethodHints: {
//                    Qt.ImhFormattedNumbersOnly
//                    //Qt.ImhExclusiveInputMask
//                }
//                onEditingFinished: {
//                    if(acceptableInput)
//                        console.log(text)
//                }
//            }

