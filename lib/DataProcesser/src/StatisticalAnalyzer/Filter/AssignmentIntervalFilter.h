#ifndef ASSIGNMENTINTERVALFILTER
#define ASSIGNMENTINTERVALFILTER

#include <vector>
#include <pqxx/pqxx>

#include "IFilter.h"

class AssignmentIntervalFilter : public IFilter
{
  private:
    bool isValidAssignmentTime(std::string& previousTime, std::string& currTime);
 
  public:
    FilterQueryColumnIndexes queryColumnIndexes;
    FilterContext filterContext;

    //used to simulate inherited vars from IFilter
    void setFilterQueryColumnIndexes(FilterQueryColumnIndexes fq) { queryColumnIndexes = fq; };
    FilterQueryColumnIndexes getFilterQueryColumnIndexes() { return queryColumnIndexes; };
    
    void setFilterContext(FilterContext fc) { filterContext = fc; };  
    FilterContext getFilterContext() { return filterContext; };


    std::vector<pqxx::result::tuple> filter(std::vector<pqxx::result::tuple> unfilteredRows);

    AssignmentIntervalFilter(FilterContext f)
    {
        filterContext = f;
    };
};

#endif