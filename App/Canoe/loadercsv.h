#ifndef LOADERCSV_H
#define LOADERCSV_H

#include "bibloader.h"
#include <QObject>

class LoaderCSV : public BibLoader
{
    Q_OBJECT
public:
    explicit LoaderCSV(Database* database);

public slots:
    void processLoad(const QString &fileName, bool reset);


    // BibLoader interface
protected slots:
    void process();
};

#endif // LOADERCSV_H
