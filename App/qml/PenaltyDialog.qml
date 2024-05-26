import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.1
import QtQml 2.12

Dialog {
    id: penaltyDialog

    focus: true
    contentHeight: parent.height*0.85
    width: contentHeight
    topMargin: (parent.height-contentHeight)/2
    leftMargin: (parent.width-width)/2
    modal: true
    closePolicy: Popup.CloseOnEscape

    property int fontSize: 18
    property double spotDiameter: 0.07
    property var model: defaultModel


    Rectangle {
        color: "black"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: titleText.bottom
    }

    Text {
        id: titleText
        color: "white"
        padding: 10
        font.pixelSize: fontSize
        anchors.left: parent.left
        anchors.top: parent.top
        text: model.title
    }

    Text {
        id: senderText
        color: "white"
        padding: 10
        horizontalAlignment: Text.Center
        font.pixelSize: fontSize
        anchors.left: titleText.right
        anchors.right: timestampText.left
        anchors.top: parent.top
        text: model.sender
    }

    Text {
        id: timestampText
        color: "white"
        padding: 10
        font.pixelSize: fontSize
        anchors.right: parent.right
        anchors.top: parent.top
        text: model.timestamp
    }


    Image {
        id: sketch
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: titleText.bottom
        anchors.bottom: dialogButtonBox.top
        anchors.topMargin: 10
        width: height
        source: "qrc:/qml/images/sketch.jpg"
        fillMode: Image.PreserveAspectFit

        Item {
            visible: model.xspot1>0 && model.xspot1<1 && model.yspot1>0 && model.yspot1<1
            width: parent.width*spotDiameter
            height: parent.height*spotDiameter
            anchors.leftMargin: parent.width*model.xspot1-width/2
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: parent.height*model.yspot1-height/2

            Rectangle {
                anchors.fill: parent
                color: "red"
                opacity: 0.35
                radius: width/2
            }
            Rectangle {
                color: "red"
                width: 1
                height: parent.height
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Rectangle {
                color: "red"
                height: 1
                width: parent.width
                anchors.verticalCenter: parent.verticalCenter
            }
        }


        Item {
            visible: model.xspot2>0 && model.xspot2<1 && model.yspot2>0 && model.yspot2<1
            width: parent.width*spotDiameter
            height: parent.height*spotDiameter
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: parent.width*model.xspot2-width/2
            anchors.topMargin: parent.height*model.yspot2-height/2

            Rectangle {
                anchors.fill: parent
                color: "red"
                opacity: 0.35
                radius: width/2
            }
            Rectangle {
                color: "red"
                width: 1
                height: parent.height
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Rectangle {
                color: "red"
                height: 1
                width: parent.width
                anchors.verticalCenter: parent.verticalCenter
            }
        }


    }


    DialogButtonBox {
        id: dialogButtonBox
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        alignment: Qt.AlignHCenter
        standardButtons: DialogButtonBox.Close
        onRejected: penaltyDialog.close()

    }

    Item {
        id: defaultModel
        property string title: "Title"
        property string sender: "Sender"
        property string timestamp: "Timestamp"
        property double xspot1: 0.5
        property double yspot1: 0.3
        property double xspot2: 0.7
        property double yspot2: 0.8
        property bool teammate: false
    }

}
