#include "simpleviewmodel.h"
#include <QQmlEngine>

SimpleViewModel::SimpleViewModel() : QObject() {

    QQmlEngine::setObjectOwnership((QObject*)this, QQmlEngine::CppOwnership);

}

void SimpleViewModel::insert(const QString& key, QVariant value) {

    _map.insert(key, value);
}

void SimpleViewModel::set(const QVariantMap& map) {

    _map = map;
    emit dataChanged();
}
