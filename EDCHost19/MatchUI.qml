import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import QtMultimedia 5.9
import QtQuick.Shapes 1.0
import my.uri 1.0 as My
import "./Resources"

ApplicationWindow {
    id: root

    width : 1200
    height : 900
    title: "进行比赛"

    readonly property color backColor: "#66bb6a"
    readonly property bool isSetPerspective: btnSetPerspective.checked
    readonly property int viewIndex: btnSetPerspective.checked ? 1 : 0

    footer: ToolBar {
        RowLayout {
            anchors.fill: parent

            ToolButton {
                text: "调试信息"
                onClicked: My.Ctrl.probeWindow.show()
            }

            ToolSeparator {}

            ToolButton {
                id: btnSetPerspective
                checkable: true
                text: "设置透视"

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
                text: "确认透视"

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

            Component.onCompleted: function() {
                source.start()
            }
        }



        Item {
            id: loc
            visible: root.isSetPerspective

            property point np1 : Qt.point(0,0)
            property point np2 : Qt.point(1,0)
            property point np3 : Qt.point(0.1,1)
            property point np4 : Qt.point(0.9,1)

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

                console.warn(np1, np2, np3, np4)
                My.Ctrl.setPerspective(1, np3, 2, np4, 3, np1, 4, np2, 6)
            }

            Shape {
                ShapePath {
                    strokeColor: "red"
                    strokeWidth: 2

                    fillColor: "#3fffffff"

                    startX: c1.x
                    startY: c1.y

                    PathLine {x: c2.x; y: c2.y}
                    PathLine {x: c4.x; y: c4.y}
                    PathLine {x: c3.x; y: c3.y}
                    PathLine {x: c1.x; y: c1.y}
                }
            }

            Circle {
                id: c1
                dragBound: loc.bound
            }

            Circle {
                id: c2
                dragBound: loc.bound
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

        StackLayout {
            id: view1
            anchors.fill: parent
            currentIndex: root.viewIndex

            RowLayout {
                id: textPanel


                Component {
                    id: lblComponent
                    Label {
                        width: 125
                        height: 25
                        verticalAlignment: Text.AlignBottom
                        horizontalAlignment: Text.AlignHCenter
                    }
                }

                /*Label {
                    id: labelScoreA
                    x: 25
                    y: 25
                    width: 125
                    height: 25
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    text: "A方得分"
                }*/
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
                    anchors.horizontalCenter: parent.horizontalCenter
                    id: btnPlusA
                    text: "A方加分"
                }

                MyButton {
                    anchors.horizontalCenter: parent.horizontalCenter
                    id: btnPenalizeA
                    text: "A方犯规"
                }

                MyButton {
                    id: btnPlusB
                    text: "B方加分"

                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MyButton {
                    id: btnPenalizeB
                    text: "B方犯规"

                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MyButton {
                    id: btnPauseResume

                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MyButton {
                    id: btnBeginEnd

                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MyButton {
                    id: btnSOA
                    text: "点球A"

                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MyButton {
                    id: btnSOB
                    text: "点球B"

                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }

            Item {

            }
        }
    }

}
