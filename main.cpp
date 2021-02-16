#include <QCoreApplication>
#include "common/logger/log.h"
#include "common/helper/signalhelper/signalhelper.h"
#include "common/coreappworker/coreappworker.h"

#include "settings.h"
#include "work1.h"

Settings _settings(
    {
        "QODBC",
        "BuildInfoFlex",
        "office.logcontrol.hu",
        9876,
        "sa",
        "Gtr7jv8fh2"
    } );

auto main(int argc, char *argv[]) -> int
{
    com::helper::SignalHelper::setShutDownSignal(com::helper::SignalHelper::SIGINT_); // shut down on ctrl-c
    com::helper::SignalHelper::setShutDownSignal(com::helper::SignalHelper::SIGTERM_); // shut down on killall

    zInfo(QStringLiteral("started"));

    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("test1"));

    QCommandLineParser parser;

    parser.setApplicationDescription(QStringLiteral("command line test1 app."));
    parser.addHelpOption();
    parser.addVersionOption();

//    const QString OPTION_IN = QStringLiteral("input");
//    const QString OPTION_OUT = QStringLiteral("output");
//    const QString OPTION_BACKUP = QStringLiteral("backup");

//    com::helper::CommandLineParserHelper::addOption(&parser, OPTION_IN, QStringLiteral("gerber file as input"));
//    com::helper::CommandLineParserHelper::addOption(&parser, OPTION_OUT, QStringLiteral("csv file as output"));
//    com::helper::CommandLineParserHelper::addOptionBool(&parser, OPTION_BACKUP, QStringLiteral("set if backup is needed"));

    parser.process(a);

//    // statikus, számítunk arra, hogy van
//    Work1::params.inFile = parser.value(OPTION_IN);
//    Work1::params.outFile = parser.value(OPTION_OUT);
//    Work1::params.isBackup = parser.isSet(OPTION_BACKUP);



    com::CoreAppWorker c(Work1::doWork,&a, &parser);
    c.run();

    auto e = QCoreApplication::exec();
    return e;
}
