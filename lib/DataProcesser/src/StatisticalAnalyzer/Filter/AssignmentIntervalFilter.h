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
    //No such thing as virtual/abstract variables which sucks;
    //because of this we cannot force children of IFIlter to have these members always.
    FilterQueryColumnIndexes queryColumnIndexes;
    FilterContext filterContext;

    std::vector<pqxx::result::tuple> filter(std::vector<pqxx::result::tuple> unfilteredRows);

    AssignmentIntervalFilter(FilterContext f)
    {
        filterContext = f;
    };
};

#endif