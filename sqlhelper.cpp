#include "sqlhelper.h"
#include "networkhelper.h"
#include "common/logger/log.h"
#include <QDirIterator>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>

SQLHelper::SQLHelper()
{

}

//https://docs.microsoft.com/en-us/sql/linux/sql-server-linux-setup-tools?view=sql-server-ver15#ubuntu
QSqlDatabase SQLHelper::Connect(const SQLSettings& s)
{
    QSqlDatabase db;
    const HostPort* h=nullptr;
    for(auto&i:s.hosts)
    {
        if(NetworkHelper::Ping(i.host)) {h=&(i);break;}
    }

    if(h)
    {
        db = QSqlDatabase::addDatabase(s.driver, "conn1");
        auto driverdir = QStringLiteral("/opt/microsoft/msodbcsql17/lib64");
        auto driverpattern = QStringLiteral("^.*libmsodbcsql-?[1-9.so]*$");
        auto driverfi = GetMostRecent(driverdir, driverpattern);
        if(!driverfi.isFile()) return db;

        auto dbname = QStringLiteral("DRIVER=%1;Server=%2,%3;Database=%4").arg(driverfi.absoluteFilePath()).arg(h->host).arg(h->port).arg(s.dbname);
        db.setDatabaseName(dbname);
        db.setUserName(s.user);
        db.setPassword(s.password);
        //volatile bool db_ok = db.open();
//        if(db_ok)
//        {
//            zTrace();
//        }
//        else
//        {
//            auto err = db.lastError().text();
//            zInfo(err);
//        }
    }
    return db;
}

void Error(const QSqlError& err)
{
    if(err.isValid()) zInfo(QStringLiteral("QSqlError: %1 - %2").arg(err.type()).arg(err.text()));
}

int SQLHelper::GetBuildNum(QSqlDatabase& db, int project)
{
    if(!db.isValid()) return -1;
    int buildnum=-1;
    QSqlError lasterr;
    QSqlQuery query(db);
    bool isok = db.open();
    if(!isok) {lasterr =db.lastError();goto end; }

    isok = query.exec(QStringLiteral("SELECT max(buildnum) FROM BuildInfoFlex.dbo.BuildInfo WHERE project=%1;").arg(project));
    if(!isok) {lasterr = query.lastError();goto end;}

    if(query.size())
    {
        query.first();
        auto a  = query.value(0);
        if(a.isNull()) buildnum = 1003; else buildnum = a.toInt()+1;
    }
    if(buildnum == -1) buildnum = 1003;

    query.prepare("INSERT INTO BuildInfoFlex.dbo.BuildInfo"
    "(version, userinfo, timestamp, product, buildnum, project, ) VALUES "
    "(:version, :userinfo, :timestamp, :product, :buildnum, :project)");

    query.bindValue(":version", "0.90");
    query.bindValue(":userinfo", "zoli@aaa");
    query.bindValue(":timestamp", "2020.02.17. 18:20");//QDateTime::currentDateTimeUtc()
    query.bindValue(":product", "PiCameraCV");
    query.bindValue(":buildnum", buildnum);
    query.bindValue(":project", "99");

    isok = query.exec();
    if(!isok) {lasterr = query.lastError();goto end;}
end:
    Error(lasterr);
    db.close();
    return buildnum;
}


QFileInfo SQLHelper::GetMostRecent(const QString& path, const QString& pattern)
{
    QFileInfo most_recent;
    auto tstamp = QDateTime(QDate(1980,1,1));// ::currentDateTimeUtc().addYears(-1);//f1.lastModified();
    QRegularExpression re(pattern);

    QDirIterator it(path);
    while (it.hasNext()) {
        auto fn = it.next();
        QFileInfo fi(fn);
        if(!fi.isFile()) continue;
        auto m = re.match(fn);
        if(!m.hasMatch()) continue;

        auto ts = fi.lastModified();
        if(ts>tstamp){ tstamp=ts; most_recent = fi;}
    }
    return most_recent;
}
