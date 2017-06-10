#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <time.h>
#include <pqxx/pqxx>

#include "../../Utilities/UtcTime.h"
#include "../../Utilities/UtcReader.h"
#include "NightOnlyFilter.h"

//does this actually override the thing?
bool NightOnlyFilter::isValidTime(UtcTime utcTime)
{
    tm timeStruct = {};
    timeStruct.tm_year = utcTime.year - 1900;
    timeStruct.tm_mon = utcTime.month - 1;
    timeStruct.tm_mday = utcTime.day;
    timeStruct.tm_hour = utcTime.hour;
    mktime( &timeStruct );

    return timeStruct.tm_hour > 22 || timeStruct.tm_hour < 6;
}