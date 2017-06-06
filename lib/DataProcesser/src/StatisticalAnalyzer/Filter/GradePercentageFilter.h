#ifndef GRADEPERCENTAGEFILTER_H
#define GRADEPERCENTAGEFILTER_H

#include <vector>
#include <pqxx/pqxx>

#include "IFilter.h"

class GradePercentageFilter : public IFilter
{
  private:

    int percentageToValue(int totalAmount);
    std::string getGradeSortingQuery();

    std::vector<int> getGradeIds();
    int getTotalAmountOfGrades();

  public:
    //No such thing as virtual/abstract variables which sucks;
    //because of this we cannot force children of IFIlter to have these members always.
    FilterQueryColumnIndexes queryColumnIndexes;
    FilterContext filterContext;

    //used to simulate inherited vars from IFilter
    void setFilterQueryColumnIndexes(FilterQueryColumnIndexes fq) { queryColumnIndexes = fq; };
    FilterQueryColumnIndexes getFilterQueryColumnIndexes() { return queryColumnIndexes; };
    
    void setFilterContext(FilterContext fc) { filterContext = fc; };  
    FilterContext getFilterContext() { return filterContext; };

    
    std::vector<pqxx::result::tuple> filter(vector<pqxx::result::tuple> unfilteredRows);

    GradePercentageFilter (FilterContext f) 
    {
        filterContext = f;
    };
};

#endif