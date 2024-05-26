#include <database.h>
#include <iostream>
#include <QSet>
#include <QStringList>

int main(int argc, char *argv[]) {

    Database db("test.db");
    QSet<QString> collectionSet = db.collectionSet();
    std::cout << "Collections: " << collectionSet.toList().join(',').toStdString() << std::endl;

    QJsonObject obj1;
    obj1.insert("key1", "value1");

    QJsonObject obj2;
    obj2.insert("key2", "value2");

    QJsonObject obj3;
    obj3.insert("key3", "value3");


    db.setValue("collec1", "obj1", obj1);
    db.setValue("collec1", "obj1", obj2);
    db.setValue("collec1", "obj3", obj3);
    db.remove("collec1", "obj3");
    db.dump("collec1");
    db.drop("collec1");

    std::cout << "Collections: " << collectionSet.toList().join(',').toStdString() << std::endl;

    db.setValue("collec1", "obj1", obj1);
    db.setValue("collec1", "obj2", obj2);
    db.setValue("collec1", "obj3", obj3);

    db.dump("collec1");

    db.remove("collec1", QStringList() << "obj2" << "obj3");

    db.dump("collec1");

    db.clear();


}
