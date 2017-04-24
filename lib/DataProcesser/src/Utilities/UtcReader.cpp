#include <vector>
#include <string>
#include <iostream>

#include "UtcReader.h"
#include "UtcTime.h" 
#include "Utilities.h"

//unit test this shit
UtcTime UtcReader::toUtcTime(std::string& s)
{
    //separate in year month day first (so, until the last '-' delimiter hit plus 2)
    std::string ymdSeparator = " ";

    //do exception handling here and shit
    try
    {
        std::string yearMonthDay = s.substr(0, s.find(ymdSeparator)); 
        std::string hoursMinutesSeconds = s.substr(s.find(ymdSeparator)+1, s.size()-1);

        auto ymdVector = Utilities::toArrayByDelim(yearMonthDay, '-');
        auto hmsVector = Utilities::toArrayByDelim(hoursMinutesSeconds, ':');

        return UtcTime (stoi(ymdVector[0]), stoi(ymdVector[1]), stoi(ymdVector[2]), 
                        stoi(hmsVector[0]), stoi(hmsVector[1]), stod(hmsVector[2]));
    } 
    catch (const std::invalid_argument& invalidArgEx) 
    {
        cout << "EXCEPTION: UtcReader::toUtcTime: stoi() got invalid argument" << endl;
    }
    catch (...)
    {
        cout << "EXCEPTION: UtcReader::toUtcTime got unknown exception" << endl;
    }
}   


