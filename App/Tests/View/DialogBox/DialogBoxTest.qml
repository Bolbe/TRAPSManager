import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.1
import QtQuick.Dialogs 1.0
import QtQml 2.12
import "../../../qml"


ApplicationWindow {
    width: 640
    height: 480
    visible: true
    color: "#888"
    title: qsTr("Hello World")


    DialogBox {
        id: dialog
        ctrl: dialogBoxQueue

    }

}
