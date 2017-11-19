import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import my.uri 1.0 as My

ApplicationWindow {
    width: 750
    height: 600
    //title: "调试信息"

    function setCameraText(text) {
        textCamera.text = text
    }

    function setSerialText(text) {
        textSerial.text = text
    }

    Component.onCompleted: {
        My.Ctrl.onCameraDebugInfoEmitted.connect(setCameraText)
        My.Ctrl.onSerialDebugInfoEmitted.connect(setSerialText)
    }

    footer: ToolBar {
        RowLayout {
            anchors.fill: parent

            ToolButton {
                text: "启用调试"

                onClicked: My.Ctrl.setCvDebugEnabled(true)
            }

            ToolButton {
                text: "禁用调试"

                onClicked: My.Ctrl.setCvDebugEnabled(false)
            }

            ToolButton {
                text: "打开串口"

                onClicked: My.Ctrl.setSerial(true)
            }

            ToolButton {
                text: "关闭串口"

                onClicked: My.Ctrl.setSerial(false)
            }

            ToolButton {
                text: "全屏主界面"

                onClicked: My.Ctrl.matchWindow.showFullScreen()
            }

            Item { Layout.fillWidth: true }

            ToolButton {
                text: "结束进程"

                onClicked: Qt.quit()
            }
        }
    }

    Label {
        id: labelCamera
        x: 25
        y: 25
        width: 325
        height: 50
        text: "相机"
    }

    TextArea {
        id: textCamera
        x: 25
        y: 100
        width: 325
        height: 400

        readOnly: true
    }

    Label {
        id: labelSerial
        x: 400
        y: 25
        width: 325
        height: 50
        text: "串口"
    }

    TextArea {
        id: textSerial
        x: 400
        y: 100
        width: 325
        height: 400

        readOnly: true
    }
}
