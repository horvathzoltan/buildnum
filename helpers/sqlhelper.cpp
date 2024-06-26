#include "helpers/sqlhelper.h"
#include "networkhelper.h"
#include "helpers/logger.h"
#include <QDirIterator>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QDateTime>
#include <QTcpSocket>
#include <QRegularExpression>

auto SQLHelper::GetDriverName() -> QString{
    //opt/microsoft/msodbcsql17/lib64/libmsodbcsql-17.10.so.2.1
    auto driverdir = QStringLiteral("/opt/microsoft/msodbcsql17/lib64");
    auto driverpattern = QStringLiteral("^.*libmsodbcsql-?[0-9.so]*$");
    auto driverfi = GetMostRecent(driverdir, driverpattern);
    if(!driverfi.isFile()) return QString();
    return driverfi.absoluteFilePath();
}

//https://docs.microsoft.com/en-us/sql/linux/sql-server-linux-setup-tools?view=sql-server-ver15#ubuntu
QSqlDatabase SQLHelper::Connect(const SQLSettings& s, const QString& name, int timeout)
{
    QSqlDatabase db;
    const HostPort* h=nullptr;
    for(auto&i:s.hosts)
    {
        //zInfo("host: "+i.host+":"+QString::number(i.port));
        if(NetworkHelper::Ping(i.host)) {
            zInfo("reachable: "+i.host+":"+QString::number(i.port));
            QTcpSocket s;
            s.connectToHost(i.host, i.port);
            auto isok = s.waitForConnected(timeout);
            if(isok){
                s.disconnectFromHost();
                if (s.state() != QAbstractSocket::UnconnectedState) s.waitForDisconnected();
                h=&(i);
                zInfo("socket ok");
                break;
            }
            else{
                zInfo("socket err");
            }
        }
        else{
            zInfo("unreachable:"+i.host);
        }
    }

    if(h)
    {
        zInfo("available host found: "+h->host+":"+QString::number(h->port));
        db = QSqlDatabase::addDatabase(s.driver, name);
        auto driverfn = GetDriverName();
        if(driverfn.isEmpty()) return db;
        auto dbname = QStringLiteral("DRIVER=%1;Server=%2,%3;Database=%4")
                .arg(driverfn,h->host).arg(h->port).arg(s.dbname);
        db.setDatabaseName(dbname);
        db.setUserName(s.user);
        db.setPassword(s.password);
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

    QSqlQuery query(db);
    bool isok = db.open();
    if(!isok) {goto end; }

    isok = query.exec(QStringLiteral("SELECT max(buildnum) FROM BuildInfoFlex.dbo.BuildInfo WHERE project=%1;").arg(project));
    if(!isok) {goto end;}

    if(query.size())
    {
        query.first();
        auto a  = query.value(0);

        if(!a.isValid() || a.isNull()) buildnum = 1003; else buildnum = a.toInt()+1;
    }
    if(buildnum == -1) buildnum = 1003;

end:
    Error(query.lastError());
    Error(db.lastError());
    db.close();
    return buildnum;
}

bool SQLHelper::SetBuildNum(QSqlDatabase& db, int project_id, const QString& user, int buildnumber, const QString & project_name)
{
    if(!db.isValid()) return -1;

    QSqlQuery query(db);
    bool isok = db.open();
    if(!isok) {goto end; }

    query.prepare("INSERT INTO BuildInfoFlex.dbo.BuildInfo"
    "(version, userinfo, timestamp, product, buildnum, project) VALUES "
    "(:version, :userinfo, :timestamp, :product, :buildnum, :project)");

    query.bindValue(QStringLiteral(":version"), "0.90");
    query.bindValue(QStringLiteral(":userinfo"), user);
    query.bindValue(QStringLiteral(":timestamp"), QDateTime::currentDateTimeUtc());
    query.bindValue(QStringLiteral(":product"), project_name);
    query.bindValue(QStringLiteral(":buildnum"), buildnumber);
    query.bindValue(QStringLiteral(":project"), project_id);

    isok = query.exec();
    if(!isok) {goto end;}
end:
    Error(query.lastError());
    Error(db.lastError());
    db.close();
    return isok;
}


QFileInfo SQLHelper::GetMostRecent(const QString& path, const QString& pattern)
{
    QFileInfo most_recent;
    static const QDate d1 = QDate(1980,1,1);
#if QT_VERSION > QT_VERSION_CHECK(5, 14, 0)
    QDateTime tstamp = d1.startOfDay(Qt::UTC);// ::currentDateTimeUtc().addYears(-1);//f1.lastModified();
#else

    QDateTime tstamp(d1, QTime(0,0));  //d1.startOfDay(Qt::UTC);// ::currentDateTimeUtc().addYears(-1);//f1.lastModified();
#endif
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

QVariant SQLHelper::GetProjId(QSqlDatabase &db, const QString &project_name)
{
    if(!db.isValid()) return -1;
    QVariant project_id;

    QSqlQuery query(db);
    bool isok = db.open();
    if(!isok) {goto end; }

    isok = query.exec(QStringLiteral("SELECT id FROM BuildInfoFlex.dbo.Projects WHERE Name='%1';").arg(project_name));
    if(!isok) {goto end;}

    if(query.size())
    {
        query.first();
        project_id= query.value(0);
    }
    else
        project_id = QVariant();

end:
    Error(query.lastError());
    Error(db.lastError());
    db.close();
    return project_id;
}
