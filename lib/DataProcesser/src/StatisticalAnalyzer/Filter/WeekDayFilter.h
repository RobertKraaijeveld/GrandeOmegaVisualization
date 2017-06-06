#ifndef WEEKDAYFILTER_H
#define WEEKDAYFILTER_H

#include <vector>
#include <pqxx/pqxx>

#include "IFilter.h"
#include "../../Utilities/UtcTime.h"


class WeekDayFilter : public IFilter
{
  private:
    bool isWeekDay(UtcTime utcTime);
    
  public:
    FilterQueryColumnIndexes queryColumnIndexes;

    //used to simulate inherited vars from IFilter
    void setFilterQueryColumnIndexes(FilterQueryColumnIndexes fq) { queryColumnIndexes = fq; };
    FilterQueryColumnIndexes getFilterQueryColumnIndexes() { return queryColumnIndexes; };


    std::vector<pqxx::result::tuple> filter(vector<pqxx::result::tuple> unfilteredRows);
};

#endif