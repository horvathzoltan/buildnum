#include "work1.h"
#include "common/logger/log.h"
#include "sqlhelper.h"
#include "settings.h"
#include "environment.h"
#include <iostream>

extern Settings _settings;
extern Environment _environment;

Work1Params Work1::params;

Work1::Work1()
{

}

int Work1::doWork()
{
    //zTrace();
    SQLHelper sqlh;
    static const QString CONN = QStringLiteral("conn1");
    auto db = sqlh.Connect(_settings._sql_settings, CONN);
    auto buildnum = sqlh.GetBuildNum(db, _settings._project);
    std::cout << QString::number(buildnum).toStdString() << '\n';
    auto isok = sqlh.SetBuildNum(db, _settings._project, _environment.user_at_host, buildnum);
    if(!isok) return 0;
    db.removeDatabase(CONN);
    zInfo(QStringLiteral("buildnum: %1").arg(buildnum));
    //zInfo(isok?"ok":"error");

    return buildnum;
}


