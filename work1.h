#ifndef WORK1_H
#define WORK1_H


#include <QString>

struct Work1Params{
public:
    QString tmpfile;
    QString ofile;
    QString projname;
};

class Work1
{
public:
    Work1();
    static int doWork();
    static Work1Params params;
private:
    static int getBuildNum(const QString& conn, int *b);
};

#endif // WORK1_H
