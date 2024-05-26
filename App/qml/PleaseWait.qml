import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.1
import QtQml 2.12

Item {
    id: pleaseWait

    anchors.fill: parent
    visible: false

    Rectangle {
        anchors.fill: parent
        opacity: 0.5
        color: "#000"
    }

    BusyIndicator {
        id: indicator
        anchors.centerIn: parent
        width: parent.width*0.1
        height: parent.height*0.1

        contentItem: Item {

            id: item
            width: indicator.width
            height: indicator.height
            x: (indicator.width-item.width)/2
            y: (indicator.height-item.height)/2
            opacity: indicator.running ? 1 : 0

            Behavior on opacity {
                OpacityAnimator {
                    duration: 250
                }
            }

            RotationAnimator {
                target: item
                running: indicator.visible && indicator.running
                from: 0
                to: 360
                loops: Animation.Infinite
                duration: 1250
            }

            Repeater {
                id: repeater
                model: 8

                Rectangle {
                    x: item.width / 2 - width / 2
                    y: item.height / 2 - height / 2
                    implicitWidth: 10
                    implicitHeight: 10
                    radius: 5
                    color: "#FFF"
                    transform: [
                        Translate {
                            y: -Math.min(item.width, item.height) * 0.5 + 5
                        },
                        Rotation {
                            angle: index / repeater.count * 360
                            origin.x: 5
                            origin.y: 5
                        }
                    ]
                }
            }
        }

    }


    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

    }


}
