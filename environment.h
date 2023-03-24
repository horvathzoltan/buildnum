#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QString>

class Environment
{
public:
    static QString Host();    
    static QString User();
    static QString UserAtHost();
};

#endif // ENVIRONMENT_H
