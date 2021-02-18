#include "work1.h"
#include "common/logger/log.h"
#include "common/helper/textfilehelper/textfilehelper.h"
#include "sqlhelper.h"
#include "settings.h"
#include "environment.h"
#include <iostream>

extern Settings _settings;
extern Environment _environment;

Work1Params Work1::params;

Work1::Work1() = default;

auto Work1::doWork() -> int
{    
    //zTrace();
    SQLHelper sqlh;
    static const QString CONN = QStringLiteral("conn1");
    auto db = sqlh.Connect(_settings._sql_settings, CONN);
    auto buildnum = sqlh.GetBuildNum(db, _settings._project);    
    auto isok = sqlh.SetBuildNum(db, _settings._project, _environment.user_at_host, buildnum);
    if(!isok) return 0;
    QSqlDatabase::removeDatabase(CONN);
    auto buildnum_str = QString::number(buildnum);
    std::cout << buildnum_str.toStdString() << '\n';
    zInfo(QStringLiteral("buildnum: %1").arg(buildnum));
    if(!params.tmpfile.isEmpty()){
        if(!params.tmpfile.endsWith(".tmp")) return 1;
        auto a = com::helper::TextFileHelper::load(params.tmpfile);
        if(a.isEmpty()) return 1;
        a = a.replace("${BUILDNUMBER}", buildnum_str);
        auto fn2 = params.tmpfile.left(params.tmpfile.length()-4);
        com::helper::TextFileHelper::save(a, fn2);
    }

    return 0;
}


