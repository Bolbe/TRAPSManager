#ifndef PENALTYREQUESTPROCESSOR_H
#define PENALTYREQUESTPROCESSOR_H

#include "requestprocessor.h"
#include "Canoe/penalty.h"

class PenaltyRequestProcessor : public RequestProcessor
{
    Q_OBJECT

signals:

    void incomingPenaltyList(QList<Penalty> penaltyList);


    // RequestProcessor interface
protected:
    QJsonDocument process(const QJsonDocument &jsonDoc);
};

#endif // PENALTYREQUESTPROCESSOR_H
