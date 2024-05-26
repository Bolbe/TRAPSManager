#ifndef DIALOGBOXQUEUE_H
#define DIALOGBOXQUEUE_H

#include <QObject>
#include <QQueue>
#include "dialogbox.h"

class DialogBoxQueue : public QObject
{
    Q_OBJECT
public:
    explicit DialogBoxQueue(QObject *parent = nullptr);

    Q_INVOKABLE void reject();
    Q_INVOKABLE void accept();
    Q_INVOKABLE void clickButton(int button);

    void push(DialogBox* dialogBox);

signals:
    void popup(QObject* dialogBox);


private:

    QQueue<DialogBox*> queue;

    void showNextDialog();

};

#endif // DIALOGBOXQUEUE_H
