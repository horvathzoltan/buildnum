#ifndef BUILDNUMBER_H
#define BUILDNUMBER_H

#define BUILDNUMBER 1033

#include <QString>

class Buildnumber{
public:
    static const QString _value;
private:
    static QString GetValue();
};

/*
// buildnumber.cpp should contain this lines below to forcing build with makefile depencencies
// usage:
// QCoreApplication::setApplicationVersion(Buildnumber::_value);
// zInfo(QStringLiteral("started ")+target+" as "+user + " build:"+ Buildnumber::_value);

#include <QString>
#include "buildnumber.h"

const QString Buildnumber::_value = GetValue();

QString Buildnumber::GetValue()
{
    return QString::number(BUILDNUMBER);
}
*/

#endif // BUILDNUMBER_H
