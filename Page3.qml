import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.0

Page {
    width: 600
    height: 400
    focusPolicy: Qt.StrongFocus

    header: Label {
        text: qsTr("Настройки")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }


    Rectangle {
        id: rectangle
        x: 2
        y: 60
        width: 106
        height: 28
        color: "#ffffff"
    }

    TextInput {
        id: textInput
        x: 0
        y: 60
        width: 106
        height: 28
        color: "#000000"
        text: ""
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

    Text {
        id: element
        x: 2
        y: 29
        width: 106
        height: 25
        text: qsTr("id группы")
        horizontalAlignment: Text.AlignHCenter
        font.family: "Tahoma"
        font.pixelSize: 21
    }

    Image {
        id: name
        x: 0
        y: 0
        width: 362
        height: 23
        source: "qrc:///example.PNG"
    }

    Button {
        id: button
        x: 2
        y: 102
        width: 104
        height: 28
        text: qsTr("Загрузить")
        font.family: "Tahoma"
        font.pointSize: 15
        onClicked: backend.loadSchedule(textInput.text)
    }
}
