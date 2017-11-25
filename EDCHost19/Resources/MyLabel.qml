import QtQuick 2.0
import QtQuick.Controls 2.2

Item {
    property string title
    property string content

    Label {
        id: labelScoreA

        text: title

        font.pointSize: 20
    }
    Label {
        anchors.top: labelScoreA.bottom
        anchors.topMargin: 20

        text: content

        font.pointSize: 20
    }
}
