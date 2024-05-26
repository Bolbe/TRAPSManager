import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.1
import QtQuick.Dialogs
import QtQml 2.12


ApplicationWindow {

    id: appWindow
    visible: true
    width: viewcontroller.appWindowWidth
    height: viewcontroller.appWindowHeight
    minimumWidth: 700
    minimumHeight: 500
    title: "TRAPSManager"

    onWidthChanged: viewcontroller.setAppWindowWidth(appWindow.width)
    onHeightChanged: viewcontroller.setAppWindowHeight(appWindow.height)


    header: ToolBar {

        Image {
            id: watchdogIcon
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height*0.6
            width: height
            sourceSize.width: height
            sourceSize.height: width
            fillMode: Image.PreserveAspectFit
            source:"images/access-point-network.svg"
            visible: false

            onVisibleChanged: if (watchdogIcon.visible) watchdogTimer.start()

            Timer {
                id: watchdogTimer
                interval: 2500
                onTriggered: watchdogIcon.visible = false
            }

        }

        Label {
            text: viewcontroller.statusText
            font.pixelSize: viewcontroller.fontSize
            anchors.left: watchdogIcon.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 20
            color: "white"
        }

        ToolButton {
            id: toolButton
            text: "<font color='#fefefe'>\u2630</font>"

            anchors.right: parent.right
            font.pixelSize: viewcontroller.fontSize*1.5
            onClicked: {
                print("You just click the tool button");
                drawer.open();
            }
        }

    }

    TRAPSDrawer {
        id: drawer
        edge: Qt.RightEdge
        width: viewcontroller.fontSize * 17
        height: parent.height

    }

    App {
        id: app
        anchors.fill: parent
    }


    PleaseWait {
        id: pleaseWait
        anchors.fill: parent
        Connections {
            target: viewcontroller
            function onTurnOnPleaseWait() { pleaseWait.visible = true }
            function onTurnOffPleaseWait() { pleaseWait.visible = false }
        }
    }

    Toaster {
        id: toaster
        fontSize: viewcontroller.fontSize
        anchors.top: parent.top
        anchors.right: parent.right
        maxHeight: app.height
        width: fontSize*15

    }

    // About / software update dialog
    SoftwareUpdate {
        id: softwareUpdate
        fontSize: viewcontroller.fontSize
        Connections {
            target: viewcontroller
            function onOpenSoftwareUpdate() { softwareUpdate.open() }
        }
        onRejected: {
            print("Dialog rejected");
        }

    }

    PenaltyDialog {
        id: penaltyDialog
        fontSize: viewcontroller.fontSize
        model: penaltyViewModel.data
        Connections {
            target: penaltyViewModel
            function onOpenPenaltyDialog() { penaltyDialog.open() }
        }
    }

    // Standard dialog box (question, info, errors)
    DialogBox {
        id: dialog
        fontSize: viewcontroller.fontSize
        ctrl: dialogBoxQueue
    }

    FileDialog {
        id: fileDialog
        currentFolder: viewcontroller.folder
        onAccepted: {
            viewcontroller.setFolder(fileDialog.selectedFile)
            viewcontroller.selectedFilePath(fileDialog.selectedFile)
        }
        onRejected: viewcontroller.selectedFilePath("")
    }

    Connections {
        target: viewcontroller
        function onPopFileChooser(title, nameFilters){
            fileDialog.title = title
            fileDialog.nameFilters = nameFilters
            fileDialog.open()
        }
        function onToast(text, delay){
            toaster.toast(text, delay)
        }
        function onWatchdog() { watchdogIcon.visible = true }
    }

    Component.onCompleted: viewcontroller.viewReady()
}
