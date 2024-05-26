#ifndef LOADERPCE_H
#define LOADERPCE_H

#include <QObject>
#include "Canoe/bibloader.h"

class Database;

class LoaderPCE : public BibLoader
{
    Q_OBJECT
public:
    explicit LoaderPCE(Database* database);

public slots:
    void processLoad(const QString &fileName, bool reset);


    // BibLoader interface
protected slots:
    void process();
};

#endif // LOADERPCE_H
