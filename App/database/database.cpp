#include "database.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QStringList>


Database::Database(const QString &filename)
{
    _filepath = filename;
    qInfo() << "Database file: "  << _filepath;
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(_filepath);
    if (!_db.open()) {
        qCritical() << "Cannot open db at " << _filepath;
        return;
    } else {
        qDebug() << "Database opened";
    }
    // check list of tables
    QSqlQuery query(_db);
    query.exec("SELECT * FROM sqlite_master WHERE type='table'");
    while (query.next()) _collectionSet << query.value("name").toString();

}

bool Database::isOpen() {
    if (!_db.isOpen()) {
        qCritical() << "Database not opened: " << _filepath;
        return false;
    }
    return true;
}

bool Database::create(const QString &collection) {
    if (!isOpen()) return false;
    qDebug() << "Creating collection " << collection;
    QSqlQuery query(_db);
    if (!query.exec(QString("CREATE table `%0` (`key` VARCHAR(30) PRIMARY KEY, `value` TEXT)").arg(collection))) {
        qCritical() << query.lastError().text();
        return false;
    }
    _collectionSet << collection;
    return true;
}

bool Database::drop(const QString &collection) {
    if (!isOpen()) return false;
    qDebug() << "Droping collection " << collection;
    QSqlQuery query(_db);
    if (!query.exec(QString("DROP TABLE `%0`").arg(collection))) {
        qCritical() << query.lastError().text();
        qCritical() << "Couldn't drop collection " << collection;
        return false;
    }
    _collectionSet.remove(collection);
    return true;

}

int Database::count(const QString &collection) {
    if (!isOpen()) return 0;
    if (!_collectionSet.contains(collection)) return 0;
    QSqlQuery query(_db);
    query.exec(QString("SELECT * FROM `%0`").arg(collection));
    return query.size();
}


bool Database::clear() {

    if (!isOpen()) return false;
    qDebug() << "Droping all tables DB";
    foreach (QString collection, _collectionSet) {
        drop(collection);
    }
    return true;

}

bool Database::setValue(const QString &collection, const QString &key, const QJsonObject& value) {

    if (!isOpen()) return false;
    if (!_collectionSet.contains(collection)) create(collection);

    QByteArray byteArray = QJsonDocument(value).toJson(QJsonDocument::Compact).replace('"', "\"\"");
    QSqlQuery query(_db);
    QString sql = QString("INSERT OR REPLACE INTO `%0` (`key`, `value`) values (\"%1\", \"%2\")")
            .arg(collection, key, QString(byteArray));
    bool ok = query.exec(sql);
    if (!ok) qCritical() << query.lastError().text();
    return ok;
}

bool Database::setValueMap(const QString& collection, const QHash<QString,QJsonObject>& valueMap) {

    if (!isOpen()) return false;
    if (!_collectionSet.contains(collection)) create(collection);
    _db.transaction();
    QSqlQuery query(_db);
    query.prepare(QString("INSERT OR REPLACE INTO `%0` (`key`, `value`) values (?,?)").arg(collection));
    foreach (QString key, valueMap.keys()) {
        QString value = QString(QJsonDocument(valueMap.value(key)).toJson(QJsonDocument::Compact).replace('"', "\""));
        query.bindValue(0, key);
        query.bindValue(1, value);
        bool result = query.exec();
        if (!result) {
            qWarning() << "Cannot insert key " << key << " with value " << value;
            qWarning() << query.lastError().text();
            break;
        }
    }
    _db.commit();
    return true;

}

QHash<QString, QJsonObject> Database::map(const QString &collection) {

    QHash<QString, QJsonObject> hash;
    if (!isOpen()) return hash;
    if (!_collectionSet.contains(collection)) return hash;
    QSqlQuery query(_db);
    query.exec(QString("SELECT * FROM `%0`").arg(collection));
    QJsonParseError parserError;
    while (query.next()) {
        QString key = query.value("key").toString();
        QByteArray value = query.value("value").toByteArray();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(value, &parserError);
        if (parserError.error==QJsonParseError::NoError) {
            hash.insert(key,jsonDoc.object());
        } else {
            qWarning() << "Cannot parse " << value;
            qWarning() << parserError.error << ": " << parserError.errorString();
        }
    }
    return hash;
}

QJsonObject Database::value(const QString &collection, const QString &key) {

    if (!isOpen()) return QJsonObject();
    if (!_collectionSet.contains(collection)) return QJsonObject();

    QSqlQuery query(_db);
    query.exec(QString("SELECT * FROM `%0` WHERE `key`==\"%1\"").arg(collection, key));
    QJsonParseError parserError;
    if (query.next()) {
        QString key = query.value("key").toString();
        QByteArray value = query.value("value").toByteArray();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(value, &parserError);
        if (parserError.error==QJsonParseError::NoError) {
            return jsonDoc.object();
        } else {
            qWarning() << "Cannot parse " << value;
            qWarning() << parserError.error << ": " << parserError.errorString();
        }
    }
    return QJsonObject(); // return empty object

}

bool Database::remove(const QString &collection, const QString &key) {
    if (!isOpen()) return false;
    if (!_collectionSet.contains(collection)) return true;
    QSqlQuery query(_db);
    bool ok = query.exec(QString("DELETE FROM `%0` WHERE `key`==\"%1\"").arg(collection, key));
    if (!ok) qCritical() << query.lastError().text();
    return ok;
}

bool Database::remove(const QString &collection, const QStringList &keyList) {
    if (!isOpen()) return false;
    if (!_collectionSet.contains(collection)) return true;
    QSqlQuery query(_db);
    _db.transaction();
    foreach (QString key, keyList) {
        bool ok = query.exec(QString("DELETE FROM `%0` WHERE `key`==\"%1\"").arg(collection, key));
        if (!ok) {
            qCritical() << query.lastError().text();
            return false;
        }
    }
    _db.commit();
    return true;
}


void Database::dump(const QString &collection) {
    qDebug() << "Dumping " << collection;
    QHash<QString, QJsonObject> tableMap = map(collection);
    foreach (QString str, tableMap.keys()) {
        qDebug() << str << ": " << QJsonDocument(tableMap.value(str)).toJson(QJsonDocument::Compact);
    }
}
