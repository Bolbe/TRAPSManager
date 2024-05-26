#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "../../../View/dialogbox.h"
#include "../../../View/dialogboxqueue.h"
#include <QQmlContext>
#include <QDebug>

int main(int argc, char *argv[]) {

    QGuiApplication app(argc, argv);

    DialogBoxQueue dialogBoxQueue;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("dialogBoxQueue", &dialogBoxQueue);
    engine.load(QUrl(QStringLiteral("qrc:/Tests/View/DialogBox/DialogBoxTest.qml")));

//    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
//                     &app, [](QObject* obj, const QUrl& objUrl) {
//        qDebug("Scene loaded");

//    }, Qt::QueuedConnection);


    DialogBox* box = new DialogBox("What a great dialog box !",
                                   "Lorem ipsum dolor sit amet, diam varius fugiat at molestie nunc, parturient phasellus. Phasellus sodales tristique, sociosqu risus vehicula varius sollicitudin lacinia, interdum sodales ultrices, maecenas integer donec mattis enim vel nec, id ligula. Dis iusto, diam viverra et non, ut nisl montes adipiscing, augue pellentesque. Leo tellus eu nullam. Elit ultricies sociis molestie eget, ante et tristique vulputate lectus aliquam eget, varius auctor nam tempus, magna soluta arcu sodales wisi pellentesque ea.",
                                   DIALOGBOX_INFORMATION,
                                   DIALOGBOX_OK | DIALOGBOX_CANCEL);

    QObject::connect(box, &DialogBox::accepted, []() {
        qDebug() << "DIALOG 1 ACCEPTED !!!";
    });

    QObject::connect(box, &DialogBox::rejected, []() {
        qDebug() << "DIALOG 1 REJECTED !!!";
    });

    DialogBox* box2 = new DialogBox("BOX 2 !",
                                   "Lorem ipsum dolor sit amet, diam varius fugiat at molestie nunc, parturient phasellus. Phasellus sodales tristique, sociosqu risus vehicula varius sollicitudin lacinia, interdum sodales ultrices, maecenas integer donec mattis enim vel nec, id ligula. Dis iusto, diam viverra et non, ut nisl montes adipiscing, augue pellentesque. Leo tellus eu nullam. Elit ultricies sociis molestie eget, ante et tristique vulputate lectus aliquam eget, varius auctor nam tempus, magna soluta arcu sodales wisi pellentesque ea.",
                                   DIALOGBOX_INFORMATION,
                                   QStringList() << "Choice 1" << "Choice 2" << "Choice 3");

    QObject::connect(box2, &DialogBox::buttonClicked, [](int button) {
        qDebug() << "DIALOG 2 CHOICE: " << button;
    });

    DialogBox* box3 = new DialogBox("What a great dialog box 3 !",
                                   "Hello",
                                   DIALOGBOX_INFORMATION,
                                   DIALOGBOX_YES | DIALOGBOX_NO);

    QObject::connect(box3, &DialogBox::accepted, []() {
        qDebug() << "DIALOG 3 ACCEPTED !!!";
    });

    QObject::connect(box3, &DialogBox::rejected, []() {
        qDebug() << "DIALOG 3 REJECTED !!!";
    });

    dialogBoxQueue.push(box);
    dialogBoxQueue.push(box2);
    dialogBoxQueue.push(box3);

    qDebug("Starting application");
    return app.exec();
}
