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
  std::vector<pqxx::result::tuple> filter(std::vector<pqxx::result::tuple> unfilteredRows)
  {
    std::vector<pqxx::result::tuple> filteredRows;

    for (pqxx::result::tuple row : unfilteredRows)
    {
      string currTime = string(row[getFilterQueryColumnIndexes().timestampIndex].c_str());
      UtcTime currUtcTime = UtcReader::toUtcTime(currTime);

      if (isValidTime(currUtcTime))
      {
        filteredRows.push_back(row);
      }
    }
    return filteredRows;
  };
};

#endif