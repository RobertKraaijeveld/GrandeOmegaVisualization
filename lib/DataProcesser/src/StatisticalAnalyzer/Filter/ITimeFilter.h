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
  virtual bool isValidTime(UtcTime utcTime){ };

public:
  FilterQueryColumnIndexes queryColumnIndexes;

  //dirty way of emulating virtual vars
  //used to simulate inherited vars from IFilter
  void setFilterQueryColumnIndexes(FilterQueryColumnIndexes fq) { queryColumnIndexes = fq; };
  FilterQueryColumnIndexes getFilterQueryColumnIndexes() { return queryColumnIndexes; };

  //default is to filter nothing. Will be overridden in children
  virtual std::vector<pqxx::result::tuple> filter(std::vector<pqxx::result::tuple> unfilteredRows){ return unfilteredRows; };

  ITimeFilter() {}
};

#endif