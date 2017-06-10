#ifndef DAYONLYFILTER_H
#define DAYONLYFILTER_H

#include <vector>
#include <iostream>
#include <pqxx/pqxx>

#include "IFilter.h"
#include "ITimeFilter.h"
#include "../../Utilities/UtcTime.h"

class DayOnlyFilter : public ITimeFilter
{
private: 
  bool isValidTime(UtcTime utcTime);

public:
  FilterQueryColumnIndexes queryColumnIndexes;

  //used to simulate inherited vars from IFilter
  void setFilterQueryColumnIndexes(FilterQueryColumnIndexes fq) { queryColumnIndexes = fq; };
  FilterQueryColumnIndexes getFilterQueryColumnIndexes() { return queryColumnIndexes; };
};

#endif