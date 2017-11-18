import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import QtMultimedia 5.9
import QtQuick.Shapes 1.0
import my.uri 1.0 as My

ApplicationWindow {
    width: 750
    height: 600
    title: "EDC19 上位机"

    background: Image {
        id: imgBack
        source: "Resources/MainBGN.png"
    }

    Component.onCompleted: My.Ctrl.matchWindow.show()

    Button {
        id: btnMatch
        x: 300
        y: 400
        width: 150
        height: 60
        text: "进入比赛"

        onClicked: {
            My.Ctrl.matchWindow.show()
        }
    }

    Button {
        id: btnDebug
        x: 575
        y: 525
        width: 100
        height: 40
        text: "调试信息"

        onClicked: {
            My.Ctrl.probeWindow.show()
        }
    }

    Button {
        id: btnPerspect
        x: 450
        y: 525
        width: 100
        height: 40
        text: "设置透视"
        enabled: false
    }
}
