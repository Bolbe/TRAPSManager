#ifndef SIMPLEVIEWMODEL_H
#define SIMPLEVIEWMODEL_H

#include <QObject>
#include <QVariantMap>

class SimpleViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantMap data READ data NOTIFY dataChanged)

public:
    SimpleViewModel();

    void insert(const QString& key, QVariant value);
    void set(const QVariantMap& map);
    QVariantMap data() const { return _map; }

signals:

    void dataChanged();

private:

    QVariantMap _map;

};

#endif // SIMPLEVIEWMODEL_H
