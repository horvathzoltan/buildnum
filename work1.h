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
    static bool BuildNum(const QString &params_tmpfile,
                         const QString &params_ofile,
                         const QString &buildnum_str);
    static bool BuildNum2(const QString &params_tmpfile,
                         const QString &buildnum_str);
private:
    static const QString _template;
    static const QRegularExpression _regexp1;
    static int getBuildNum(const QString& conn, int *b);
};

#endif // WORK1_H
