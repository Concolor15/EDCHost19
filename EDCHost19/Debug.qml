import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import my.uri 1.0 as My

ApplicationWindow {
    width: 750
    height: 600
    id: root
    title: "调试信息"

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
        ColumnLayout {
            anchors.fill: parent

            RowLayout {
                Layout.fillWidth: true

                ToolButton {
                    text: "获取参数"

                    onClicked: txtParam.text = My.Config.getProcConfigByString()
                }

                TextField {
                    Layout.fillWidth: true
                    id: txtParam
                }

                ToolButton {
                    text: "设置参数"

                    onClicked: My.Config.setProcConfigByString(txtParam.text)
                }

            }

            RowLayout {
                Layout.fillWidth: true

                ToolButton {
                    text: "打开/关闭摄像头"

                    onClicked: My.Ctrl.toggleCamera()
                }

                Item {Layout.fillWidth: true}

                TextField {
                    id: serialName
                }

                ToolButton {
                    text: "打开串口"

                    onClicked: My.Ctrl.setSerial(true, serialName.text)
                }

                ToolButton {
                    text: "关闭串口"

                    onClicked: My.Ctrl.setSerial(false, "")
                }
            }

            RowLayout {
                Layout.fillWidth: true

                ToolButton {
                    text: "启用调试"

                    onClicked: My.Config.setCvDebugEnabled(true)
                }

                ToolButton {
                    text: "禁用调试"

                    onClicked: My.Config.setCvDebugEnabled(false)
                }

                ToolButton {
                    text: "显示主界面"
                    onClicked: My.Ctrl.matchWindow.showNormal()
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
        width: 325

        anchors.top: labelCamera.bottom
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15

        font.family: "monospace"

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
        width: 325

        anchors.top: labelCamera.bottom
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15

        font.family: "monospace"

        readOnly: true
    }
}
