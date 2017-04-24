#include <string>

#include "UtcTime.h" 

std::string UtcTime::ToString()
{
    std::string formattedMinus = "-";
    std::string formattedColon = ":";
    std::string formattedSpace = " ";
    
    std::string ymd = std::to_string(year) + formattedMinus + std::to_string(month) + formattedMinus + std::to_string(day);
    std::string hms = std::to_string(hour) + formattedColon +  std::to_string(minute) + formattedColon + std::to_string(second);

    return ymd + formattedSpace + hms;
}