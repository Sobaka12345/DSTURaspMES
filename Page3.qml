import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.0

Page {
    id: opts
    width: 600
    height: 400
    focusPolicy: Qt.StrongFocus

    header: Label {
        text: qsTr("Настройки")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }


    ColumnLayout {
        width: parent.width
        Layout.alignment: Qt.AlignHCenter
        x: 0
        y: 0

        Image {
            id: groupId
            width: opts.width
            Layout.fillWidth: true
            source: "qrc:///example.PNG"
        }

        ColumnLayout {
            width: parent.width
            Layout.alignment: Qt.AlignHCenter
            Text {
                id: element
                text: qsTr("id группы")
                Layout.preferredHeight: 25
                Layout.preferredWidth: 139
                horizontalAlignment: Text.AlignHCenter
                font.family: "Tahoma"
                font.pixelSize: 21
            }

            Rectangle {
                id: rectangle
                width: 139
                height: 28
                color: "#ffffff"

                TextInput {
                    id: textInput
                    color: "#000000"
                    text: ""
                    anchors.fill: parent
                    Layout.preferredHeight: 28
                    Layout.preferredWidth: 139
                    maximumLength: 5
                    cursorVisible: true
                    font.weight: Font.ExtraLight
                    horizontalAlignment: Text.AlignLeft
                    selectionColor: "#00801c"
                    font.family: "Tahoma"
                    font.pixelSize: 21
                    validator: IntValidator{bottom: 0; top: 99999;}
                    focus: true
                    inputMethodHints: Qt.ImhDigitsOnly
                }
            }

            Button {
                id: button
                text: qsTr("Загрузить")
                Layout.preferredHeight: 28
                Layout.preferredWidth: 139
                font.family: "Tahoma"
                font.pointSize: 15
                onClicked: backend.loadSchedule(textInput.text)
            }
        }
        Image {
            id: prepName
            width: opts.width
            Layout.fillWidth: true
            source: "qrc:///example2.png"
        }
        ColumnLayout {
            Layout.alignment: Qt.AlignHCenter
            width: 280

            Text {
                id: elementP
                text: qsTr("#!@$% Преподавателя")
                anchors.fill: parent
                Layout.preferredHeight: 25
                Layout.preferredWidth: 139
                horizontalAlignment: Text.AlignHCenter
                font.family: "Tahoma"
                font.pixelSize: 21
            }

            Rectangle {
                id: rectangleP
                width: parent.width
                height: 28
                color: "#ffffff"

                TextInput {
                    id: textInputP
                    color: "#000000"
                    text: ""
                    clip: true
                    anchors.fill: parent
                    Layout.preferredHeight: 28
                    Layout.preferredWidth: parent.width
                    maximumLength: 100
                    cursorVisible: true
                    font.weight: Font.ExtraLight
                    horizontalAlignment: Text.AlignLeft
                    selectionColor: "#008080"
                    font.family: "Tahoma"
                    font.pixelSize: 21
                    focus: true
                }
            }

            Button {
                id: buttonP
                text: qsTr("Загрузить")
                Layout.preferredHeight: 28
                Layout.preferredWidth: 139
                font.family: "Tahoma"
                font.pointSize: 15
                onClicked: backend.loadSchedule(textInputP.text)
            }
        }
    }
}





















/*##^## Designer {
    D{i:7;anchors_x:0;anchors_y:30}D{i:13;anchors_height:28;anchors_width:139}
}
 ##^##*/
