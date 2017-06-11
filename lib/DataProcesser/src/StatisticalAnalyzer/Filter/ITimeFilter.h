#ifndef ITimeFilter_H
#define ITimeFilter_H

#include <vector>
#include <memory>
#include <pqxx/pqxx>

#include "../../Utilities/UtcTime.h"
#include "../../Utilities/UtcReader.h"
#include "IFilter.h"

/**
  A ITimeFilter is a special kind of IFilter which filters rows based on their timestamp only.
  Each derived class of ITimeFilter only has to override 'isValidTime'; the 'filter' method
  which calls said method for each row is the same for each instance of ITimeFilter and is thusly not virtual.
 */

class ITimeFilter : IFilter
{
private:
  //default is to filter nothing.
  virtual bool isValidTime(UtcTime utcTime){ return true; };

public:
  FilterQueryColumnIndexes queryColumnIndexes;

  //dirty way of emulating virtual vars
  //used to simulate inherited vars from IFilter
  void setFilterQueryColumnIndexes(FilterQueryColumnIndexes fq) { queryColumnIndexes = fq; };
  FilterQueryColumnIndexes getFilterQueryColumnIndexes() { return queryColumnIndexes; };

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

  ITimeFilter() {}
};

#endif