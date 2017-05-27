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

class IFilter
{
  protected:
  public:
    //These are probably just hidden ><
    FilterQueryColumnIndexes queryColumnIndexes;
    FilterContext filterContext;

    virtual std::vector<pqxx::result::tuple> filter(std::vector<pqxx::result::tuple> unfilteredRows) = 0;
};

#endif