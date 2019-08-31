import QtQuick 2.12
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3



Page {
    id: lol
    width: 600
    height: 400

    header: Label {
        text: qsTr("Верхняя неделя")
        font.pointSize: 16
        background: Rectangle {
            color: "white"
                    anchors.fill: parent
        }
        styleColor: "#00000000"
        font.family: "Verdana"
        padding: 10
    }

    Rectangle{
        id: container
        anchors.fill: parent

        Component {
            id: sectionHeading
            Rectangle {
                width: parent.width
                height: childrenRect.height
                color: "lightsteelblue"

                Text {
                    text: section
                    font.bold: true
                    font.pixelSize: 20
                    wrapMode: "WordWrap"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }

        ListView {
            id: listView
            anchors.fill: parent
            model: backend.upDataList

            delegate: RowLayout {
                width: parent.width
                spacing: 0

                Label {
                    Layout.fillHeight: true

                    Layout.preferredHeight: implicitHeight + 10
                    Layout.preferredWidth: parent.width * 0.2
                    background: Rectangle {
                        border.color: "black"
                        color: "grey"
                    }
                    text: model.modelData.time
                    wrapMode: "WordWrap"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                Label {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.preferredHeight: implicitHeight + 10
                    background: Rectangle { border.color: "black" }
                    text: model.modelData.info
                    wrapMode: "WordWrap"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            section.property: "modelData.day"
            section.criteria: ViewSection.FullString
            section.delegate: sectionHeading

        }
    }
}





















