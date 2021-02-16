#ifndef SQLHELPER_H
#define SQLHELPER_H

#include <QString>

class SQLHelper
{
public:
    struct SQLSettings
    {
        QString driver;// = "QODBC";
        QString dbname;// = "BuildInfoFlex";
        QString dbhost;// = "172.16.1.5";
        int port;
        QString user;// = "sa";
        QString password;//= "Gtr7jv8fh2";
    };

    SQLHelper();
    void Connect(const SQLSettings& s);
};

#endif // SQLHELPER_H
