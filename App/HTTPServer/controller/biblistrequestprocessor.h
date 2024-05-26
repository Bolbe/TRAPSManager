#ifndef BIBLISTREQUESTPROCESSOR_H
#define BIBLISTREQUESTPROCESSOR_H

#include <QObject>
#include "requestprocessor.h"

class BibListModel;

class BibListRequestProcessor : public RequestProcessor
{
    Q_OBJECT

public:
    BibListRequestProcessor();
    void setBibListModel(BibListModel* bibList) { _bibList = bibList; }

signals:


    // RequestProcessor interface
protected:
    QJsonDocument process(const QJsonDocument &jsonDoc);

private:

    BibListModel* _bibList;

};

#endif // BIBLISTREQUESTPROCESSOR_H
