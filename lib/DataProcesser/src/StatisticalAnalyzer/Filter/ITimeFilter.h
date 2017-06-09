#ifndef ITimeFilter_H
#define ITimeFilter_H

#include <vector>
#include <memory>
#include <pqxx/pqxx>

#include "../../Utilities/UtcTime.h"
#include "../../Utilities/UtcReader.h"
#include "IFilter.h"

//Marker pattern, not great
class ITimeFilter : IFilter
{
private:
  //default is to filter nothing
  virtual bool isValidTime(UtcTime utcTime){ return true; };

public:
  FilterQueryColumnIndexes queryColumnIndexes;

  //dirty way of emulating virtual vars
  //used to simulate inherited vars from IFilter
  void setFilterQueryColumnIndexes(FilterQueryColumnIndexes fq) { queryColumnIndexes = fq; };
  FilterQueryColumnIndexes getFilterQueryColumnIndexes() { return queryColumnIndexes; };

  //default is to filter nothing. Will be overridden in children
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