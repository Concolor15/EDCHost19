import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import QtMultimedia 5.9
import my.uri 1.0 as My

Window {
    width : 1500
    height : 950
    title: "进行比赛"

    My.Filter {
        id: filter
    }

    VideoOutput {
        id: vid
        source: My.Ctrl.camera
        filters: [filter]
        x: 25
        y: 175
        width: 1280
        height: 720

        Component.onCompleted: function() {
            source.start()
        }
    }

    Rectangle {
        id: textPanel
        x: 0
        y: 0
        width: parent.width
        height: 150
        color: "#66bb6a"
    }

    Item {
        id: btnPanel
        x:1300
        y:0
        width: 200
        height: 950

        Button {
            id: btnPlusA
            text: "A方加分"
            x: 50
            y: 175
            width: 100
            height: 40
        }

        Button {
            id: btnPenalizeA
            text: "A方犯规"
            x: 50
            y: 250
            width: 100
            height: 40
        }

        Button {
            id: btnPlusB
            text: "B方加分"
            x: 50
            y: 325
            width: 100
            height: 40
        }

        Button {
            id: btnPenalizeB
            text: "B方犯规"
            x: 50
            y: 400
            width: 100
            height: 40
        }

        Button {
            id: btnPauseResume
            x: 50
            y: 550
            width: 100
            height: 40
        }

        Button {
            id: btnBeginEnd
            x: 50
            y: 550
            width: 100
            height: 40
        }

        Button {
            id: btnSOA
            text: "点球A"
            x: 50
            y: 625
            width: 100
            height: 40
        }

        Button {
            id: btnSOB
            text: "点球B"
            x: 50
            y: 700
            width: 100
            height: 40
        }
    }
}
