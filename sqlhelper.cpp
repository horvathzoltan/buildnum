#include "sqlhelper.h"
#include "networkhelper.h"
#include "common/logger/log.h"
#include <QSqlDatabase>

SQLHelper::SQLHelper()
{

}

//https://docs.microsoft.com/en-us/sql/linux/sql-server-linux-setup-tools?view=sql-server-ver15#ubuntu
void SQLHelper::Connect(const SQLSettings& s)
{
    auto isDB = NetworkHelper::Ping(s.dbhost);
    if(isDB)
    {
        QSqlDatabase db = QSqlDatabase::addDatabase(s.driver, "conn1");
        //db.setHostName(s.dbhost);
        //db.setPort(s.port);
        db.setDatabaseName(QStringLiteral("DRIVER=/opt/microsoft/msodbcsql17/lib64/libmsodbcsql-17.7.so.1.1;Server=%1,%2;Database=%3").arg(s.dbhost).arg(s.port).arg(s.dbname));
        db.setUserName(s.user);
        db.setPassword(s.password);
        volatile bool db_ok = db.open();
        if(db_ok)
        {
            zInfo("sql ok");
        }
    }
}
