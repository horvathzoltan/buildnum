#ifndef SETTINGS_H
#define SETTINGS_H
#include <QString>
#include "sqlhelper.h"

class Settings
{
public:
    SQLHelper::SQLSettings _sql_settings;
    Settings(const SQLHelper::SQLSettings&a);
};

#endif // SETTINGS_H
