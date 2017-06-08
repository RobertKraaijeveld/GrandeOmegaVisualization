#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <time.h>
#include <pqxx/pqxx>

#include "../../Utilities/UtcTime.h"
#include "../../Utilities/UtcReader.h"
#include "../../DatabaseInteracter/DatabaseInteracter.h"
#include "WeekendDayFilter.h"

//does this actually override the thing?
bool WeekendDayFilter::isValidTime(UtcTime utcTime)
{
    tm timeStruct = {};
    timeStruct.tm_year = utcTime.year - 1900;
    timeStruct.tm_mon = utcTime.month - 1;
    timeStruct.tm_mday = utcTime.day;
    timeStruct.tm_hour = 12; //  To avoid any doubts about summer time, etc.
    mktime( &timeStruct );

    //tm_wday returns an int; 1-6 is week, 0 and 7 are sunday/saturday
    return timeStruct.tm_wday == 0 || timeStruct.tm_wday == 7;
}