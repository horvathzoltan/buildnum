#include <QCoreApplication>
#include "common/logger/log.h"
#include "common/helper/signalhelper/signalhelper.h"
#include "common/coreappworker/coreappworker.h"
#include "common/helper/CommandLineParserHelper/commandlineparserhelper.h"
#include "settings.h"
#include "environment.h"
#include "work1.h"
#include "buildnumber.h"

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
    com::helper::SignalHelper::setShutDownSignal(com::helper::SignalHelper::SIGINT_); // shut down on ctrl-c
    com::helper::SignalHelper::setShutDownSignal(com::helper::SignalHelper::SIGTERM_); // shut down on killall

//    zInfo(QStringLiteral("started: %1").arg(Buildnumber::buildnum));
    zInfo(QStringLiteral("started: %1").arg(BUILDNUMBER));

    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("test12"));

    QCommandLineParser parser;

    parser.setApplicationDescription(QStringLiteral("command line test1 app."));
    parser.addHelpOption();
    parser.addVersionOption();

    const QString OPTION_TMP = QStringLiteral("template");
    const QString OPTION_OUT = QStringLiteral("output");
//    const QString OPTION_BACKUP = QStringLiteral("backup");

    com::helper::CommandLineParserHelper::addOption(&parser, OPTION_TMP, QStringLiteral("template file"));
    com::helper::CommandLineParserHelper::addOption(&parser, OPTION_OUT, QStringLiteral("file as output"));
//    com::helper::CommandLineParserHelper::addOptionBool(&parser, OPTION_BACKUP, QStringLiteral("set if backup is needed"));

    parser.process(a);

//    // statikus, számítunk arra, hogy van
    Work1::params.tmpfile = parser.value(OPTION_TMP);
    Work1::params.ofile = parser.value(OPTION_OUT);
//    Work1::params.isBackup = parser.isSet(OPTION_BACKUP);

    //TODO a parser is nem kell, a paraméterek kellenek
    com::CoreAppWorker c(Work1::doWork, &a, &parser);
    c.run();

    auto e = QCoreApplication::exec();    
    return e;
}
