#ifndef TRAPSDB_H
#define TRAPSDB_H

#include "database/database.h"

#define DB_BIB "bibs"
#define DB_LOCKS "locks"
#define DB_TIMES "times"
#define DB_PENALTIES "penalties"

class TRAPSDB : public Database
{
public:
    TRAPSDB(const QString& filename);


};

#endif // TRAPSDB_H
