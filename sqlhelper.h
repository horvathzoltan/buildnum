#ifndef SQLHELPER_H
#define SQLHELPER_H

#include <QString>
#include <QFileInfo>
#include <QSqlDatabase>

class SQLHelper
{
public:
    struct HostPort
    {
        QString host;// = "172.16.1.5";
        int port;
    };

    struct SQLSettings
    {
        QString driver;// = "QODBC";
        QString dbname;// = "BuildInfoFlex";
        QVector<HostPort> hosts;
        QString user;// = "sa";
        QString password;//= "Gtr7jv8fh2";
    };

    SQLHelper();
    QSqlDatabase Connect(const SQLSettings& s);

    static QFileInfo GetMostRecent(const QString &path, const QString &pattern);
    int GetBuildNum(QSqlDatabase &db, int project);
};

#endif // SQLHELPER_H
