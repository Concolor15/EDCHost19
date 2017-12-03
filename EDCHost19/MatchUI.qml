import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import QtMultimedia 5.9
import QtQuick.Shapes 1.0
import my.uri 1.0
import my.uri 1.0 as My
import "./Resources"

ApplicationWindow {
    id: root

    width : 1200
    height : 900
    title: "进行比赛\uE32A"

    readonly property color backColor: "#66bb6a"
    readonly property bool isSetPerspective: btnSetPerspective.checked
    readonly property int viewIndex: btnSetPerspective.checked ? 1 : 0

    property bool canStart:
        Logic.status === Logic.NotStart ||
        Logic.status === Logic.Finished

    property bool onProgress:
        Logic.status === Logic.Running ||
        Logic.status === Logic.Paused

    property bool canPause: Logic.status === Logic.Running

    Component.onCompleted: function() {
        Ctrl.setPerspective(loc.np1, loc.np2, loc.np3, loc.np4)
        Ctrl.toggleCamera()
    }

    footer: ToolBar {
        RowLayout {
            anchors.fill: parent

            ToolButton {
                text: "调试信息\uE32A"
                onClicked: My.Ctrl.probeWindow.show()
            }

            ToolSeparator {}

            ToolButton {
                id: btnSetPerspective
                checkable: true
                text: "设置区域"

                onToggled: {
                    if (checked)
                        loc.reset()
                }
            }

            ToolSeparator {}

            Item {
                Layout.fillWidth: true
            }

            ToolButton {
                visible: btnSetPerspective.checked
                text: "确认区域"

                onClicked: {
                    loc.set()
                    btnSetPerspective.checked = false
                }
            }
        }
    }

    My.Filter {
        id: filter
    }

    Item {
        id: vidcanvas

        anchors.left: parent.left
        anchors.right: view2rect.left
        anchors.top: view1rect.bottom
        anchors.bottom: parent.bottom

        anchors.margins: 20

        VideoOutput {
            id: vid
            source: My.Ctrl.camera
            filters: [filter]
            anchors.fill: parent

            onSourceChanged: {
                if (source != null)
                    source.start();
            }
        }

        Item {
            id: adorner
            property point ballPos: vid.mapPointToItem(Logic.rawBallPos)
            property point carAPos: vid.mapPointToItem(Logic.rawCarAPos)
            property point carBPos: vid.mapPointToItem(Logic.rawCarBPos)

            readonly property color car1Color: "#FF0000"
            readonly property color car2Color: "#0000FF"

            anchors.fill: parent
            Shape {
                ShapePath {
                    strokeColor: "red"
                    strokeWidth: 2

                    fillColor: root.isSetPerspective ? "#3fffffff" : "transparent"

                    startX: c1.x
                    startY: c1.y

                    PathLine {x: c2.x; y: c2.y}
                    PathLine {x: c4.x; y: c4.y}
                    PathLine {x: c3.x; y: c3.y}
                    PathLine {x: c1.x; y: c1.y}
                }
            }

            BallIndicator {
                x: adorner.ballPos.x
                y: adorner.ballPos.y
            }

            CarIndicator {
                x: adorner.carAPos.x
                y: adorner.carAPos.y
                color: adorner.car2Color
            }

            CarIndicator {
                x: adorner.carBPos.x
                y: adorner.carBPos.y
                color: adorner.car1Color
            }
        }

        Item {
            id: loc
            visible: root.isSetPerspective

            property point np1 : Qt.point(0,0)
            property point np2 : Qt.point(1,0)
            property point np3 : Qt.point(0,1)
            property point np4 : Qt.point(1,1)

            readonly property rect bound: vid.contentRect

            function reset() {
                var p1 = vid.mapNormalizedPointToItem(np1)
                var p2 = vid.mapNormalizedPointToItem(np2)
                var p3 = vid.mapNormalizedPointToItem(np3)
                var p4 = vid.mapNormalizedPointToItem(np4)
                c1.x = p1.x; c1.y = p1.y;
                c2.x = p2.x; c2.y = p2.y;
                c3.x = p3.x; c3.y = p3.y;
                c4.x = p4.x; c4.y = p4.y;
            }

            function set() {
                np1 = vid.mapPointToSourceNormalized(Qt.point(c1.x, c1.y))
                np2 = vid.mapPointToSourceNormalized(Qt.point(c2.x, c2.y))
                np3 = vid.mapPointToSourceNormalized(Qt.point(c3.x, c3.y))
                np4 = vid.mapPointToSourceNormalized(Qt.point(c4.x, c4.y))

                My.Ctrl.setPerspective(np1, np2, np3, np4)
            }

            Circle {
                id: c1
                dragBound: loc.bound
                color: "#000000"
            }

            Circle {
                id: c2
                dragBound: loc.bound
                color: "#FFAAAA"
            }

            Circle {
                id: c3
                dragBound: loc.bound
            }

            Circle {
                id: c4
                dragBound: loc.bound
            }
        }
    }

    Rectangle {
        id: view1rect
        x: 0
        y: 0
        width: parent.width
        height: 150
        color: root.backColor

        RowLayout {
            id: textPanel
            anchors.fill: parent
            anchors.topMargin: 20

            Item {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.preferredWidth: 150
                Layout.fillHeight: true
                Label {
                    id: labelTime

                    text: "时间"

                    font.pointSize: 24
                }
                Label {
                    anchors.top: labelTime.bottom
                    anchors.topMargin: 20

                    text: Logic.elapsedTime

                    font.pointSize: 24
                }
            }

            MyLabel {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.preferredWidth: 150
                Layout.fillHeight: true

                title: "红方得分"
                content: Logic.scoreA
            }

            MyLabel {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.preferredWidth: 150
                Layout.fillHeight: true

                title: "蓝方得分"
                content: Logic.scoreB
            }

            MyLabel {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.preferredWidth: 150
                Layout.fillHeight: true

                enabled: Logic.evilA !== 0

                title: "红方邪恶"
                content: Logic.evilA
            }

            MyLabel {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.preferredWidth: 150
                Layout.fillHeight: true

                enabled: Logic.evilB !== 0

                title: "蓝方邪恶"
                content: Logic.evilB
            }

            MyLabel {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.preferredWidth: 150
                Layout.fillHeight: true

                enabled: Logic.restStopA > 0

                title: "红方强停"
                content: Logic.restStopA
            }

            MyLabel {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.preferredWidth: 150
                Layout.fillHeight: true

                enabled: Logic.restStopB > 0

                title: "蓝方强停"
                content: Logic.restStopB
            }
        }

    }

    Rectangle {
        id: view2rect
        anchors.top: view1rect.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: 150

        color: root.backColor

        StackLayout {
            anchors.fill: parent
            currentIndex: root.viewIndex

            Column {
                id: btnPanel
                anchors.fill: parent

                topPadding: 20
                spacing: 20

                MyButton {
                    id: btnPauseResume
                    text: root.canPause ? "暂停" : "继续"

                    enabled: !root.canStart

                    anchors.horizontalCenter: parent.horizontalCenter

                    onClicked: {
                        if (root.canPause)
                            Logic.pause()
                        else
                            Logic.resume()
                    }
                }

                MyButton {
                    id: btnBeginEnd
                    text: root.canStart ? "开始" : "结束"

                    anchors.horizontalCenter: parent.horizontalCenter

                    onClicked: {
                        if (root.canStart)
                            Logic.start()
                        else
                            Logic.stop()
                    }
                }

                MyButton {
                    id: btnSOA
                    text: "红方点球"

                    enabled: !root.onProgress

                    anchors.horizontalCenter: parent.horizontalCenter

                    onClicked: Logic.setSide(0)
                }

                MyButton {
                    id: btnSOB
                    text: "蓝方点球"

                    enabled: !root.onProgress

                    anchors.horizontalCenter: parent.horizontalCenter

                    onClicked: Logic.setSide(1)
                }

                MyButton {
                    anchors.horizontalCenter: parent.horizontalCenter
                    id: btnPlusA
                    text: "红方得分"

                    onClicked: Logic.setScore(0, Logic.scoreA+1)
                }

                MyButton {
                    id: btnPlusB
                    text: "蓝方加分"

                    anchors.horizontalCenter: parent.horizontalCenter

                    onClicked: Logic.setScore(1, Logic.scoreB+1)
                }


                Label {
                    text: Logic.shootSide==0 ? "点球方：红方" : "点球方：蓝方"
                    color: Logic.shootSide==0 ? "red" : "blue"
                }

            }

            Item {

            }
        }
    }

}
