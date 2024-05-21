#include "work1.h"
#include "helpers/logger.h"
#include "helpers/sqlhelper.h"
#include "helpers/textfilehelper.h"
#include "qregularexpression.h"
#include "settings.h"
#include "environment.h"
#include <QCoreApplication>
#include <QDir>
#include <QRegularExpression>
#include <QVariant>
#include <iostream>

extern Settings _settings;
extern Environment _environment;

const QRegularExpression Work1::_regexp1(R"(#define\s+BUILDNUMBER\s+[\w\S]*)");
const QString Work1::_template(R"(#ifndef BUILDNUMBER_H
#define BUILDNUMBER_H

#define BUILDNUMBER 1022

#include <QString>

struct Buildnumber{ static QString toString(); };

#endif // BUILDNUMBER_H

/*
// buildnumber.cpp should contain this lines below to forcing build with makefile depencencies
#include <QString>
#include "buildnumber.h"

QString Buildnumber::toString(){return QString::number(BUILDNUMBER);}
*/
)");

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
    zInfo(QStringLiteral("%1:%2").arg(params.projname).arg(buildnum));

    bool tempFile = !params.tmpfile.isEmpty();
    if(tempFile){
        if(params.tmpfile.endsWith(".tmp"))
        {
            BuildNum(params.tmpfile, params.ofile, buildnum_str);
        } else {
            BuildNum2(params.tmpfile, buildnum_str);
        }
    }
    if(!params.deploy.isEmpty()){
        DeployBuildnum(params.deploy, buildnum_str);
    }
    return 0;
}

bool Work1::BuildNum(const QString& tmpFile, const QString& outputFile, const QString& buildnum_str){
    //if(!tmpFile.endsWith(".tmp")) return 4;
    QString a;

    bool loaded = TextFileHelper::Load(tmpFile, &a);
    if(!loaded){
        zInfo(QStringLiteral("load failed: ")+TextFileHelper::LastError());
        return false;
    }
    if(a.isEmpty()) return false;
    a = a.replace("${BUILDNUMBER}", buildnum_str);
    auto fn2 = tmpFile.left(tmpFile.length()-4);
    QString of="";
    if(outputFile.isEmpty())
        of = QFileInfo(fn2).fileName();
    else
        of = outputFile;

    bool saved = TextFileHelper::Save(a, of);
    if(!saved){
        zInfo(QStringLiteral("save failed: ")+of)
        return false;
    }
    return true;
}

bool Work1::BuildNum2(const QString& tmpFile, const QString& buildnum_str){
    QString a;
    bool loaded = TextFileHelper::Load(tmpFile, &a);
    if(!loaded){
        zInfo(QStringLiteral("load failed: ")+TextFileHelper::LastError());
        a = _template;
    } else{
        if(a.isEmpty()) a = _template;
    }

    auto match = _regexp1.match(a);
    if(!match.hasMatch()) return false;
    a.replace(match.capturedStart(), match.capturedLength(), "#define BUILDNUMBER "+buildnum_str);

    bool saved = TextFileHelper::Save(a, tmpFile);
    if(!saved){
        zInfo(QStringLiteral("save failed: ")+TextFileHelper::LastError());
        return false;
    }
    return true;
}


auto Work1::getBuildNum(const QString& conn, int *b) -> int
{
    SQLHelper sqlh;
    auto db = sqlh.Connect(_settings._sql_settings, conn, params.sql_timeout);
    QVariant project_id_v = sqlh.GetProjId(db, params.projname);
    if(!project_id_v.isValid()) return 2;
    if(project_id_v.isNull()) return 2;
    int project_id = project_id_v.toInt();
    auto buildnum = sqlh.GetBuildNum(db, project_id);
    auto uah = Environment::UserAtHost();
    auto isok = sqlh.SetBuildNum(db, project_id, uah, buildnum, params.projname);
    if(!isok) return 3;

    if(b) *b = buildnum;
    return 0;
}

bool Work1::DeployBuildnum(const QString& deploy, const QString buildnumber)
{
    if(deploy.isEmpty()) return false;
    if(buildnumber.isEmpty()) return false;

    QDir deploy_dir(deploy);
    if(!deploy_dir.exists()) return false;

    QString fn = deploy_dir.absoluteFilePath("buildnumber.txt");

    bool saved = TextFileHelper::Save(buildnumber, fn);
    if(!saved){
        zInfo(QStringLiteral("save failed: ")+TextFileHelper::LastError());
        return false;
    }
    return true;
}

