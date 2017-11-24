import QtQuick 2.7
import QtQuick.Shapes 1.0

Item {
    id: root
    readonly property alias dragArea: dragArea
    property rect dragBound
    default property color color: "#FFFFFF"

    Shape {
        ShapePath {
            id:path
            fillColor: color
            readonly property double r : 5.0

            startX: -r
            startY: 0

            PathArc {
                relativeX: 2*path.r
                relativeY: 0
                radiusX: path.r
                radiusY: path.r
                direction: PathArc.Counterclockwise
            }

            PathArc {
                relativeX: -2*path.r
                relativeY: 0
                radiusX: path.r
                radiusY: path.r
                direction: PathArc.Counterclockwise
            }
        }
    }

    MouseArea {
        id: dragArea
        x: -10
        y: -10
        width: 20
        height: 20

        drag.target: parent

        drag.minimumX: dragBound.left
        drag.maximumX: dragBound.right
        drag.minimumY: dragBound.top
        drag.maximumY: dragBound.bottom

    }
}
