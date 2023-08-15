#include <QCoreApplication>
#include "helpers/logger.h"
#include "helpers/signalhelper.h"
#include "helpers/commandlineparserhelper.h"
#include "helpers/coreappworker.h"

#include "settings.h"
#include "environment.h"
#include "work1.h"
#include "buildnumber.h"

/*
git rm -rf --cached .
git rm --cached *.pro.*
git commit -m ".gitignore is now working"

*/
/*
sql elérés megjavítása
/opt/microsoft/msodbcsql17/lib64/libmsodbcsql-17.10.so.2.1
sql tool telepítése
https://docs.microsoft.com/en-us/sql/linux/sql-server-linux-setup-tools?view=sql-server-ver15#ubuntu


curl https://packages.microsoft.com/keys/microsoft.asc | sudo apt-key add -
curl https://packages.microsoft.com/config/ubuntu/20.04/prod.list | sudo tee /etc/apt/sources.list.d/msprod.list
sudo apt-get update
sudo apt-get install mssql-tools unixodbc-dev
sudo apt-get update
sudo apt-get install mssql-tools
echo 'export PATH="$PATH:/opt/mssql-tools/bin"' >> ~/.bash_profile
echo 'export PATH="$PATH:/opt/mssql-tools/bin"' >> ~/.bashrc
source ~/.bashrc
*/

// projects/run/CommandLineArguments
// -p buildnumber

// projects/run/CommandLineArguments
/*
buildnum build:
-p buildnumber -t %{sourceDir}/buildnumber.h
*/

/*
Insole02 build:
Custom Process Step: buildnum_ -t /home/zoli/buildnumber.h.tmp -o buildnumber.h -p Insole02
Command: ~/buildnum_
Arguments: -t ~/buildnumber.h.tmp -o buildnumber.h -p Insole02
Working directory: %{sourceDir}
*/

Settings _settings(
    {
        "QODBC",
        "BuildInfoFlex",
            {{"office.logcontrol.hu", 9876}, {"172.16.1.5", 1433}},
        "sa",
        "Gtr7jv8fh2"
    } );

Environment _environment;

auto main(int argc, char *argv[]) -> int
{
    SignalHelper::setShutDownSignal(SignalHelper::SIGINT_); // shut down on ctrl-c
    SignalHelper::setShutDownSignal(SignalHelper::SIGTERM_); // shut down on killall
    Logger::Init(Logger::ErrLevel::INFO, Logger::DbgLevel::TRACE, true, true);

    zInfo(QStringLiteral("started: %1").arg(BUILDNUMBER));

    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("test12"));

    QCommandLineParser parser;

    parser.setApplicationDescription(QStringLiteral("command line test1 app."));
    parser.addHelpOption();
    parser.addVersionOption();

    const QString OPTION_TMP = QStringLiteral("template");
    const QString OPTION_OUT = QStringLiteral("output");
    const QString OPTION_PROJNAME = QStringLiteral("project");
    const QString OPTION_DEPLOY = QStringLiteral("deploy");

    CommandLineParserHelper::addOption(&parser, OPTION_TMP, QStringLiteral("template file"));
    CommandLineParserHelper::addOption(&parser, OPTION_OUT, QStringLiteral("file as output"));
    CommandLineParserHelper::addOption(&parser, OPTION_PROJNAME, QStringLiteral("project name"));
    CommandLineParserHelper::addOption(&parser, OPTION_DEPLOY, QStringLiteral("deploy dir"));

    parser.process(a);

//    // statikus, számítunk arra, hogy van
    Work1::params.tmpfile = parser.value(OPTION_TMP);
    Work1::params.ofile = parser.value(OPTION_OUT);
    Work1::params.projname = parser.value(OPTION_PROJNAME);
    Work1::params.deploy = parser.value(OPTION_DEPLOY);

    //TODO a parser is nem kell, a paraméterek kellenek
    CoreAppWorker c(Work1::doWork, &a, &parser);
    volatile auto errcode = c.run();

    switch(errcode)
    {
        case 1: zInfo("no project name use option -p name"); break;
        case 2: zInfo("project not found"); break;
    }

    auto e = QCoreApplication::exec();    
    return e;
}
