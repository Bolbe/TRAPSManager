import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.1
import QtQml 2.12

Dialog {
    id: dialogRoot

    property var ctrl
    property alias message: contentMessage.text
    property var customButtonLabels: []
    property int presetButtons: -1
    property string iconName
    property int fontSize: 18

    focus: true
    width: parent.width
    contentHeight: textItem.height>dialogIcon.implicitHeight?textItem.height:dialogIcon.implicitHeight
    topMargin: (parent.height-contentHeight-header.implicitHeight-footer.implicitHeight*2)/2
    modal: true
    closePolicy: Popup.NoAutoClose

    Connections {
        target: ctrl
        function onPopup(dialogBox) {
            dialogRoot.title = dialogBox.title
            dialogRoot.iconName = dialogBox.iconName
            dialogRoot.message = dialogBox.message
            dialogRoot.presetButtons = dialogBox.presetButtons
            dialogRoot.customButtonLabels = dialogBox.customButtonLabels
            textItem.forceActiveFocus()
            dialogRoot.open()

        }
    }


    header: Label {
        text: title
        font.pixelSize: fontSize*1.2
        height: fontSize*2
        font.weight: Font.Medium
        padding: 15
    }

    footer: Item {

        visible: presetButtons>0
        implicitHeight: childrenRect.height

        Row {

            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 15

            Button {
                visible: presetButtons & 0b00000001
                text: qsTr("  OK  ")
                onClicked: {
                    print("Clicking OK")
                    acceptDialog()
                }
            }
            Button {
                visible: presetButtons & 0b00000010
                text: qsTr("  Annuler  ")
                onClicked: {
                    print("Clicking Abort")
                    rejectDialog()
                }
            }
            Button {
                visible: presetButtons & 0b00000100
                text: qsTr("  Oui  ")
                onClicked: {
                    print("Clicking Yes")
                    acceptDialog()
                }
            }
            Button {
                visible: presetButtons & 0b00001000
                text: qsTr("  Non  ")
                onClicked: {
                    print("Clicking No")
                    rejectDialog()
                }
            }
            Button {
                visible: presetButtons & 0b00010000
                text: qsTr("  Fermer  ")
                onClicked: {
                    print("Clicking Close")
                    acceptDialog()
                }
            }

        }

    }

    Image {
        id: dialogIcon
        anchors.left: parent.left
        anchors.top: parent.top
        height: 80
        width: height
        visible: iconName!=""
        sourceSize.width: height
        sourceSize.height: width
        fillMode: Image.PreserveAspectFit
        source:visible?"images/"+iconName:""
    }

    Item {

        id: textItem
        anchors.left: dialogIcon.right
        anchors.leftMargin: 15
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.top: parent.top

        height: contentMessage.implicitHeight+customList.height
        focus: true

        Keys.onEscapePressed: {
            print("Escape pressed")
            rejectDialog()
        }

        Label {
            id: contentMessage
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            font.pixelSize: fontSize
            wrapMode: Text.Wrap
        }


        ListView {
            id: customList
            focus: true
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: contentMessage.bottom
            anchors.topMargin: 15
            height: customButtonLabels.length>0?customList.contentHeight+15:0
            visible: customButtonLabels.length>0

            model: customButtonLabels
            orientation: ListView.Vertical
            delegate: MenuRow {
                width: parent.width
                fontSize: dialogRoot.fontSize
                height: fontSize*2.5
                horizontalAlignment: Text.AlignHCenter
                selected: index === customList.currentIndex
                text: modelData
                onHovered: customList.currentIndex = index
                onClicked: {
                    dialogRoot.close()
                    ctrl.clickButton(customList.currentIndex)
                }

            }
        }

    }

    function rejectDialog() {
        dialogRoot.close()
        ctrl.reject()
    }

    function acceptDialog() {
        dialogRoot.close()
        ctrl.accept()
    }

}
