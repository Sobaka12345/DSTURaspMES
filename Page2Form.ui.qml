import QtQuick 2.12
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3



Page {
    id : kek
    width: 600
    height: 400

    header: Label {
        text: qsTr("Нижняя неделя")
        color: "white"
        font.pointSize: 16
        background: Rectangle {
            color: "#4f4e47"
            anchors.fill: parent
        }
        styleColor: "#00000000"
        font.family: "Verdana"
        padding: 10
    }

    Rectangle{
        id: containerb
        anchors.fill: parent
        color: "#f7f7e2"

        Component {
            id: sectionHeadingb
            Rectangle {
                width: parent.width
                height: childrenRect.height
                color:  "#466874"

                Text {
                    padding: 5
                    leftPadding: 10
                    color: "white"
                    text: section
                    font.bold: false
                    font.pixelSize: 20
                    wrapMode: "WordWrap"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }

        ListView {
            id: listViewb
            anchors.fill: parent
            model: backend.downDataList

            delegate: RowLayout {
                width: parent.width
                spacing: 0

                Label {
                    Layout.fillHeight: true

                    Layout.preferredHeight: implicitHeight + 10
                    Layout.preferredWidth: parent.width * 0.2
                    background: Rectangle {
                        border.color: "#301c0c"
                        color: "#9f926e"
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
                    background: Rectangle {
                        border.color: "#301c0c"
                        color: "#f7f7e2"
                    }
                    text: model.modelData.info
                    wrapMode: "WordWrap"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            section.property: "modelData.day"
            section.criteria: ViewSection.FullString
            section.delegate: sectionHeadingb

        }
    }
}





















