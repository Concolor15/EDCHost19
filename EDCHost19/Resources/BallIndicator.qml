import QtQuick 2.0
import QtQuick.Shapes 1.0

Shape {
    id: ballIndicator
    readonly property color color1: "#FFFF00"

    Rectangle {
        color: "#3FFFFFFF"
        x: -20; y: -20
        width: 40; height: 40
    }

    ShapePath
    {
        fillColor: ballIndicator.color1
        strokeWidth: -1
        startX:10; startY:10
        PathLine {x:20; y:15}
        PathLine {x:15; y:20}
    }
    ShapePath
    {
        fillColor: ballIndicator.color1
        strokeWidth: -1
        startX:-10; startY:10
        PathLine {x:-20; y:15}
        PathLine {x:-15; y:20}
    }
    ShapePath
    {
        fillColor: ballIndicator.color1
        strokeWidth: -1
        startX:-10; startY:-10
        PathLine {x:-20; y:-15}
        PathLine {x:-15; y:-20}
    }
    ShapePath
    {
        fillColor: ballIndicator.color1
        strokeWidth: -1
        startX:10; startY:-10
        PathLine {x:20; y:-15}
        PathLine {x:15; y:-20}
    }
}
