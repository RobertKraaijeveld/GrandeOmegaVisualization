#ifndef ASSIGNMENTINTERVALFILTER
#define ASSIGNMENTINTERVALFILTER

#include <vector>
#include <pqxx/pqxx>

class AssignmentIntervalFilter : public IFilter
{
  private:
    bool isValidAssignmentTime(std::string &previousTime, std::string &currTime);
    std::vector<pqxx::result::tuple> getRowsWithValidAssignmentTimes(std::vector<pqxx::result::tuple>& gradeFilteredRows);

  public:
    //No such thing as virtual/abstract variables which sucks;
    //because of this we cannot force children of IFIlter to have these members always.
    FilterQueryColumnIndexes queryColumnIndexes;
    FilterContext filterContext;

    //maybe make this take a vector<pqxx::result::tuple> instead?
    vector<pqxx::result::tuple> filter(vector<pqxx::result::tuple> unfilteredRows);

    AssignmentIntervalFilter(FilterQueryColumnIndexes &q, FilterContext &f)
    {
        queryColumnIndexes = q;
        filterContext = f;
    };
};

#endif