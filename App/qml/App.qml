import QtQuick.Window 2.2
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.1

import "../spreadsheet"

FocusScope {
    id: app
    focus: true

    property int fontSize: viewcontroller.fontSize

    width: 800
    height: 600


    BibScheduling {
        id: bibScheduling
        anchors.left: parent.left
        anchors.top: parent.top
        height: fontSize*2.5
        orderSelection: "number"
        onScheduling: function(selection) { bibListModel.setScheduling(selection) }
        onTabPressed: displayTimeBox.forceActiveFocus()
    }

    CheckBox {
        id: displayTimeBox
        text: "Afficher chronos"
        height: bibScheduling.height
        anchors.left: bibScheduling.right
        anchors.top: parent.top
        anchors.leftMargin: 10
        checked: bibListModel.chronoVisible
        font.pixelSize: viewcontroller.fontSize*0.9
        onCheckedChanged: bibListModel.setChronoVisible(displayTimeBox.checked)
        hoverEnabled: true
        ToolTip.delay: 1000
        ToolTip.visible: hovered
        ToolTip.text: "Affiche les colonnes départ, arrivée et chrono"

    }

    CheckBox {
        id: teamModeTimeBox
        text: "Mode équipe"
        height: bibScheduling.height
        anchors.left: displayTimeBox.right
        anchors.top: parent.top
        anchors.leftMargin: 10
        checked: bibListModel.teamMode
        font.pixelSize: viewcontroller.fontSize*0.9
        onCheckedChanged: bibListModel.setTeamMode(teamModeTimeBox.checked)
        hoverEnabled: true
        ToolTip.delay: 1000
        ToolTip.visible: hovered
        ToolTip.text: "Mode équipe: 3 pénalités par porte"

    }

    SpreadSheet {
        id: bibGrid

        anchors.top: bibScheduling.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: comPanel.top
        fontSize: app.fontSize
        spreadSheetModel: bibListModel
        rowHeight: app.fontSize*1.8
        headerHeight: rowHeight
        rowHighlightHovering: true
        cellHighlightHovering: false
        firstColumnHighlightHovering: 6
        lastColumnHighlightHovering: 31


        primaryColor: Material.color(Material.Blue, Material.Shade700)
        colorShade100: Material.color(Material.Blue, Material.Shade100)
        colorShade700: Material.color(Material.Blue, Material.Shade700)


    }

    ComPanel {
        id: comPanel

        height: 206+5.45*viewcontroller.fontSize
        y: app.height - comPanel.height
        anchors.left: parent.left
        anchors.right: parent.right
        fontSize: viewcontroller.fontSize
        //incomingDataModel: incomingDataController
        onToggleOpenShut: {
            if (comPanel.opened) shut()
            else open()
        }
        onOpenRequested: {
            if (!opened) open()
        }

        onTabPressed: menuPanel.open()
        Keys.onEscapePressed: bibGrid.forceActiveFocus()

        function shut() {
            comPanel.opened = false
            comPanel.y = app.height - app.fontSize*2
        }

        function open() {
            comPanel.opened = true
            comPanel.y = app.height - comPanel.height
        }


    }

    Keys.onPressed: function(event) {
        print("App level: Pressed key "+event.key)
        if (event.modifiers & Qt.ControlModifier) switch (event.key) {
            case Qt.Key_Minus:
                viewcontroller.incFontSize(-1);
                break;
            case Qt.Key_Plus:
                viewcontroller.incFontSize(1);
                break;
        }
        else switch (event.key) {
            case Qt.Key_M:
                menuPanel.open()
                break;
            case Qt.Key_C: {
                if (comPanel.shown) comPanel.hide(); else comPanel.show();
                break;
            }
        }

    }

    onHeightChanged: {
        comPanel.shut()
    }


}
