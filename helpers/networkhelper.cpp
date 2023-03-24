#include "networkhelper.h"
#include "helpers/processhelper.h"


auto NetworkHelper::Ping(const QString& ip) -> bool
{
    auto out = ProcessHelper::Execute("ping", {"-c1","-W1",ip});
    return !out.exitCode;
}
