import QtQuick 2.0
import QtQuick.Shapes 1.0

Shape {
    id: root
    property color color

    Rectangle {
        color: "#3FFFFFFF"
        x: -15; y: -15
        width: 30; height: 30
    }

    ShapePath
    {
        fillColor: root.color
        strokeWidth: -1
        startX: 0; startY: 10
        PathLine {x:10; y:0}
        PathLine {x:0; y:-10}
        PathLine {x:-10; y:0}
    }
}
