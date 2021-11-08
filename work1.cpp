#include "work1.h"
#include "common/logger/log.h"
#include "common/helper/textfilehelper/textfilehelper.h"
#include "sqlhelper.h"
#include "settings.h"
#include "environment.h"
#include <QVariant>
#include <iostream>

extern Settings _settings;
extern Environment _environment;

Work1Params Work1::params;

Work1::Work1() = default;

auto Work1::doWork() -> int
{    
    if(params.projname.isEmpty()) return 1;

    static const QString CONN = QStringLiteral("conn1");
    int buildnum;
    auto err = getBuildNum(CONN, &buildnum);
    if(err) return err;
    QSqlDatabase::removeDatabase(CONN);
    auto buildnum_str = QString::number(buildnum);
    std::cout << buildnum_str.toStdString() << '\n';
    zInfo(QStringLiteral("buildnum ver: %1").arg(buildnum));
    if(!params.tmpfile.isEmpty()){
        if(!params.tmpfile.endsWith(".tmp")) return 4;
        auto a = com::helper::TextFileHelper::load(params.tmpfile);
        if(a.isEmpty()) return 5;
        a = a.replace("${BUILDNUMBER}", buildnum_str);
        auto fn2 = params.tmpfile.left(params.tmpfile.length()-4);        
        QString of="";
        if(params.ofile.isEmpty())
            of = QFileInfo(fn2).fileName();
        else
            of = params.ofile;
        if(com::helper::TextFileHelper::save(a, of)) zInfo(QStringLiteral("saved: ")+of)
    }

    return 0;
}


auto Work1::getBuildNum(const QString& conn, int *b) -> int
{
    SQLHelper sqlh;
    auto db = sqlh.Connect(_settings._sql_settings, conn);
    QVariant project_id_v = sqlh.GetProjId(db, params.projname);
    if(!project_id_v.isValid()) return 2;
    if(project_id_v.isNull()) return 2;
    int project_id = project_id_v.toInt();
    auto buildnum = sqlh.GetBuildNum(db, project_id);
    auto isok = sqlh.SetBuildNum(db, project_id, _environment.user_at_host, buildnum, params.projname);
    if(!isok) return 3;

    if(b) *b = buildnum;
    return 0;
}

