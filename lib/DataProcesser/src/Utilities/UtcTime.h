#ifndef UTCTIME_H
#define UTCTIME_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;

//grandomega uses YYYY-MM-DD HH:MM:SS.MS
class UtcTime {
    private:
        
    public:
        int year;
        int month;
        int day;

        int hour;
        int minute;
        double second;

        UtcTime(int y, int m, int d, int h, int min, double s)
        {
            year = y;
            month = m;
            day = d;
            hour = h;
            minute = min;
            second = s;
        };
        std::string ToString();
};


#endif
