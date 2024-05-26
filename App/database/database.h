#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QSqlDatabase>
#include <QHash>
#include <QSet>
#include <QJsonObject>

class Database {

public:

    explicit Database(const QString& filename);

    bool setValue(const QString& collection, const QString& key, const QJsonObject& value);
    bool setValueMap(const QString& collection, const QHash<QString,QJsonObject>& valueMap);

    QHash<QString, QJsonObject> map(const QString& collection);
    QJsonObject value(const QString& collection, const QString& key);
    bool remove(const QString& collection, const QString& key);
    bool remove(const QString& collection, const QStringList& keyList);
    QSet<QString> collectionSet() const { return _collectionSet; }
    bool drop(const QString &collection);
    int count(const QString& collection);
    bool clear();

    void dump(const QString& collection);

private:

    QString _filepath;
    QSet<QString> _collectionSet;
    QSqlDatabase _db;

    bool isOpen();
    bool create(const QString& collection);



};

#endif // DATABASE_H
