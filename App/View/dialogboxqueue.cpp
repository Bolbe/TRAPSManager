#include "dialogboxqueue.h"
#include <QDebug>

DialogBoxQueue::DialogBoxQueue(QObject *parent) : QObject(parent) {

}

void DialogBoxQueue::reject() {
    qDebug() << "Rejecting dialog box";
    if (queue.isEmpty()) return;
    DialogBox* dialogBox = queue.dequeue();
    if (dialogBox!=0) {
        qDebug() << "Emit dialog box" << dialogBox->id() << " is rejected";
        emit dialogBox->rejected();
        dialogBox->deleteLater();
    }
    showNextDialog();
}

void DialogBoxQueue::accept() {
    qDebug() << "Accepting dialog box";
    if (queue.isEmpty()) return;
    DialogBox* dialogBox = queue.dequeue();
    if (dialogBox!=0) {
        qDebug() << "Emit dialog box" << dialogBox->id() << " is accepted";
        emit dialogBox->accepted();
        dialogBox->deleteLater();
    }
    showNextDialog();
}

void DialogBoxQueue::clickButton(int button) {
    qDebug() << "Dialog box choice: " << button;
    if (queue.isEmpty()) return;
    DialogBox* dialogBox = queue.dequeue();
    if (dialogBox!=0) {
        qDebug() << "Emit dialog box" << dialogBox->id() << " is has button clicked";
        emit dialogBox->buttonClicked(button);
        dialogBox->deleteLater();
    }
    showNextDialog();
}

void DialogBoxQueue::push(DialogBox *dialogBox) {

    queue.enqueue(dialogBox);
    if (queue.size()==1) showNextDialog();
}

void DialogBoxQueue::showNextDialog() {

    if (queue.isEmpty()) {
        qDebug("No more dialog box to show");
        return;
    }
    DialogBox* dialogBox = queue.head();
    emit popup(dialogBox);
}
