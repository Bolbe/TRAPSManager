#include "dialogbox.h"
#include <QDebug>

int DialogBox::_count = 0;

DialogBox::DialogBox(const QString& title, const QString& message, const QString& iconName, int presetButtons) : QObject(),
    _id(_count++),
    _title(title),
    _message(message),
    _iconName(iconName),
    _presetButtons(presetButtons)
{
    qDebug() << "Creating DialogBox" << _id;
}

DialogBox::DialogBox(const QString &title, const QString &message, const QString &iconName, const QStringList& customButtonLabels) : QObject(),
    _id(_count++),
    _title(title),
    _message(message),
    _iconName(iconName),
    _presetButtons(0),
    _customButtonLabels(customButtonLabels)
{
    qDebug() << "Creating DialogBox" << _id;
}

DialogBox::~DialogBox() {
    qDebug() << "Destroying DialogBox" << _id;
}
