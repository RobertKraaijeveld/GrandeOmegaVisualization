#ifndef WEEKDAYFILTER_H
#define WEEKDAYFILTER_H

#include <vector>
#include <iostream>
#include <pqxx/pqxx>

#include "IFilter.h"
#include "ITimeFilter.h"
#include "../../Utilities/UtcTime.h"

class WeekDayFilter : public ITimeFilter
{
private:
  bool isValidTime(UtcTime utcTime);

public:
  FilterQueryColumnIndexes queryColumnIndexes;

  //used to simulate inherited vars from IFilter
  void setFilterQueryColumnIndexes(FilterQueryColumnIndexes fq) { queryColumnIndexes = fq; };
  FilterQueryColumnIndexes getFilterQueryColumnIndexes() { return queryColumnIndexes; };


  //This has to be redefined in the children of ITimeFilter each time:/
};

#endif