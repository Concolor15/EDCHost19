import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Window 2.3
import my.uri 1.0 as My

Window {
    width: 750
    height: 600
    //title: "调试信息"

    function setCameraText(text) {
        textCamera.text = text
    }

    Component.onCompleted: {
        My.Ctrl.onCameraDebugInfoEmitted.connect(setCameraText)
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
