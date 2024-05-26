import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.1


FocusScope {

    id: bibScheduling
    property string orderSelection
    focus: true
    width: childrenRect.width

    signal scheduling(int selection)
    signal tabPressed

    Rectangle {
        anchors.fill: parent
        color: "white"
    }

    Rectangle {
        id: bibCountBg
        height: parent.height
        width: bibCount.width+height
        color: "gray"

        Text {
            id: bibCount
            color: "white"
            font.pixelSize: viewcontroller.fontSize
            height: parent.height
            anchors.left: parent.left
            anchors.leftMargin: height/2
            verticalAlignment: Text.AlignVCenter
            font.weight: Font.Bold
            text: viewcontroller.bibCount+" dossards"
        }
    }

    Text {
        id: bibOrderText
        color: "black"
        text: "ordonnés selon :"
        font.pixelSize: viewcontroller.fontSize*0.9
        height: parent.height
        verticalAlignment: Text.AlignVCenter
        anchors.left: bibCountBg.right
        anchors.leftMargin: 10
    }

    ComboBox {
        id: bibOrderBox
        focus: true
        height: parent.height
        width: viewcontroller.fontSize*20
        anchors.left: bibOrderText.right
        anchors.leftMargin: 20
        font.pixelSize: viewcontroller.fontSize
        currentIndex: bibListModel.scheduling
        model: [ "Numéro de dossard", "Heure de départ", "Rang dans le fichier d'origine" ]
        onCurrentIndexChanged: scheduling(currentIndex)

    }

}
