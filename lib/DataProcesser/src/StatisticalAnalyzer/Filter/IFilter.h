#ifndef IFILTER_H
#define IFILTER_H

#include <vector>
#include <memory>
#include <pqxx/pqxx>

//no guarantee that fields will be set, which sucks
struct FilterQueryColumnIndexes
{
    int studentIdColumnIndex;
    int timestampIndex;
    FilterQueryColumnIndexes() {}
};

struct FilterContext
{
    double timeBetweenAssignmentsThreshold = 1;
    double upperPercentageOfGradesToBeSelected = 100;
};


/**
    An IFilter filters PQXX database rows.
    Each IFilter has a FilterQueryColumnIndexes member (simulated by virtual getters/setters) which is used to define which
    query column index maps to which kind of property, and a filtercontext object which is used to pass parameters to the filter. 
*/


class IFilter
{
  public:
    //dirty way of emulating virtual vars
    virtual void setFilterQueryColumnIndexes(FilterQueryColumnIndexes fq){};
    virtual FilterQueryColumnIndexes getFilterQueryColumnIndexes(){};
    virtual void setFilterContext(FilterContext fc){};  
    virtual FilterContext getFilterContext(){};
  
    virtual std::vector<pqxx::result::tuple> filter(std::vector<pqxx::result::tuple> unfilteredRows) = 0;
};

#endif