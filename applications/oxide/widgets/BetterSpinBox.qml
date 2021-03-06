import QtQuick 2.10
import QtQuick.Controls 2.4

SpinBox {
    id: control
    up.indicator: Rectangle {
        x: control.mirrored ? 0 : parent.width - width
        height: parent.height
        implicitWidth: 40
        implicitHeight: 40
        visible: control.enabled && control.value < control.to
        color: "white"
        border.color: "black"

        Text {
            text: "+"
            font.pixelSize: control.font.pixelSize * 2
            color: "black"
            anchors.fill: parent
            fontSizeMode: Text.Fit
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
    down.indicator: Rectangle {
        x: control.mirrored ? parent.width - width : 0
        height: parent.height
        implicitWidth: 40
        implicitHeight: 40
        visible: control.enabled && control.value > control.from
        color: "white"
        border.color: "black"

        Text {
            text: "-"
            font.pixelSize: control.font.pixelSize * 2
            color: "black"
            anchors.fill: parent
            fontSizeMode: Text.Fit
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
