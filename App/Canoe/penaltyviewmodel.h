#ifndef PENALTYVIEWMODEL_H
#define PENALTYVIEWMODEL_H

#include "View/simpleviewmodel.h"

class PenaltyViewModel : public SimpleViewModel
{
    Q_OBJECT

public:

    static const QString title;
    static const QString sender;
    static const QString timestamp;
    static const QString teammate;
    static const QString xspot1;
    static const QString yspot1;
    static const QString xspot2;
    static const QString yspot2;

    PenaltyViewModel();

signals:

    void openPenaltyDialog();
};

#endif // PENALTYVIEWMODEL_H
