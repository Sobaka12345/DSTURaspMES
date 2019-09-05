import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.0

Page {
    id: opts
    width: 600
    height: 400
    focusPolicy: Qt.ClickFocus
    background: Rectangle {
        color: "#f7f7e2"
    }

    header: Label {
        text: qsTr("Настройки")
        color: "white"
        font.pointSize: 16
        background: Rectangle {
            color: "#4f4e47"
            anchors.fill: parent
        }
        font.family: "Verdana"
        padding: 10

        Text {
            id: element1
            anchors.right: parent.right
            y: parent.height * 0.15
            width: 115
            height: 29
            color: "#ffffff"
            text: qsTr("ver 2.1 (\\/)_(*,,,*)_(\\/)<br>by Egor MustLove")
            font.pixelSize: 11
        }
    }

    Flickable{
        width: parent.width
        height: parent.height
        flickableDirection:Flickable.VerticalFlick


        ColumnLayout {
            spacing: 0.5
            anchors.bottomMargin: 0
            anchors.fill: parent

            Text {
                id: element
                text: qsTr("Группа")
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                Layout.preferredHeight: 25
                Layout.preferredWidth: 139
                horizontalAlignment: Text.AlignHCenter
                font.family: "Tahoma"
                font.pixelSize: 21
            }

            Rectangle {
                id: rectangle
                height: 26
                color: "#ffffff"
                border.color: "#a4a4a4"
                border.width: 1
                Layout.fillWidth: true

                TextInput {

                    id: textInput
                    height: 3
                    color: "#000000"
                    text: ""
                    anchors.fill: parent
                    validator: RegExpValidator { regExp: /[0-9А-Яа-я ]+/ }
                    cursorVisible: true
                    horizontalAlignment: Text.AlignLeft
                    selectionColor: "#00801c"
                    font.family: "Tahoma"
                    font.pixelSize: 21
                }
            }

            Button {
                id: button
                height: 32
                text: qsTr("Загрузить")
                Layout.fillWidth: true
                font.family: "Tahoma"
                font.pointSize: 15
                onClicked: backend.loadId(textInput.text, 1)
            }

            Text {
                id: elementP
                text: qsTr("Поиск преподавателя")
                wrapMode: Text.WordWrap
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.preferredHeight: 32
                horizontalAlignment: Text.AlignHCenter
                font.family: "Tahoma"
                font.pixelSize: 21
            }

            RowLayout {
                height: 32

                ComboBox {
                    id: comboPrep
                    height: 32
                    font.pixelSize: 16
                    Layout.preferredHeight: 26
                    transformOrigin: Item.Center
                    Layout.fillWidth: true
                    validator: RegExpValidator { regExp: /[А-Яа-я ]+/ }
                    editable: true
                    model: backend.prepList
                    textRole: "name"
                    focusPolicy: Qt.NoFocus
                    onAccepted: activeFocus = false


                    AnimatedImage {
                        id: load
                        visible: false
                        z: 1000
                        x:parent.width - 50
                        y: 3
                        width: 20
                        height: 20
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:///load.gif"
                    }

                    Label {

                        Layout.preferredHeight: implicitHeight + 10
                        Layout.preferredWidth: parent.width
                        background: Rectangle {
                            border.color: "#301c0c"
                            color: "#9f926e"
                        }
                        wrapMode: "WordWrap"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }


                    onModelChanged:
                    {
                        load.visible = false
                        backend.setIndex(model[0].id)
                    }
                    onActivated: {
                        backend.setIndex(model[index].id)
                    }
                }

                Button {
                    id: search
                    height: 32
                    text: "Поиск"
                    Layout.preferredHeight: 26
                    Layout.preferredWidth: 100
                    onClicked: {
                        load.visible = comboPrep.editText != ""
                        backend.searchPreps(comboPrep.editText)
                    }
                }
            }





            Button {
                id: buttonP
                height: 32
                Layout.alignment: Text.AlignHCenter
                text: qsTr("Загрузить")
                Layout.fillWidth: true
                font.family: "Tahoma"
                font.pointSize: 15
                onClicked: backend.loadSchedule("")
            }

            Button {
                id: updateB
                padding: 20
                text: qsTr("Обновить расписание")
                Layout.fillWidth: true
                font.family: "Tahoma"
                font.pointSize: 15
                onClicked: {
                    backend.update()
                }
            }


        }
    }
}



















































