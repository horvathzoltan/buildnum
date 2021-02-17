#include "work1.h"
#include "common/logger/log.h"
#include "sqlhelper.h"
#include "settings.h"

extern Settings _settings;

Work1Params Work1::params;

Work1::Work1()
{

}

int Work1::doWork()
{
    zTrace();
    SQLHelper sqlh;
    auto db = sqlh.Connect(_settings._sql_settings);
    auto buildnum = sqlh.GetBuildNum(db, _settings._project);
    zInfo(QStringLiteral("buildnum: %1").arg(buildnum));
    return 1;
}


