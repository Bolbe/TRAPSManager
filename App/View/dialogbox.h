#ifndef DIALOGBOX_H
#define DIALOGBOX_H

#include <QObject>

#define DIALOGBOX_OK     0b00000001
#define DIALOGBOX_CANCEL 0b00000010
#define DIALOGBOX_YES    0b00000100
#define DIALOGBOX_NO     0b00001000
#define DIALOGBOX_CLOSE  0b00010000

#define DIALOGBOX_QUESTION "question.svg"
#define DIALOGBOX_INFORMATION "information.svg"
#define DIALOGBOX_ALERT "alert.svg"


class DialogBox : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title CONSTANT)
    Q_PROPERTY(QString message READ message CONSTANT)
    Q_PROPERTY(QString iconName READ iconName CONSTANT)
    Q_PROPERTY(int presetButtons READ presetButtons CONSTANT)
    Q_PROPERTY(QStringList customButtonLabels READ customButtonLabels CONSTANT)

public:
    explicit DialogBox(const QString& title, const QString& message, const QString& iconName, int presetButtons);
    explicit DialogBox(const QString& title, const QString& message, const QString& iconName, const QStringList& customButtonLabels);
    ~DialogBox();

    QString title() const { return _title; }
    QString message() const { return _message; }
    QString iconName() const { return _iconName; }
    int presetButtons() const { return _presetButtons; }
    QStringList customButtonLabels() const { return _customButtonLabels; }
    int id() const { return _id; }

signals:

    void rejected();
    void accepted();
    void buttonClicked(int);

private:

    int _id;
    QString _title;
    QString _message;
    QString _iconName;
    int _presetButtons;
    QStringList _customButtonLabels;

    static int _count;

};

#endif // DIALOGBOX_H
