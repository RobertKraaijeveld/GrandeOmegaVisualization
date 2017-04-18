require_dependency 'dataprocesserglue'
require 'rubygems'
require 'json'

class HomeController < ApplicationController
  include DataProcesserCoupling

  def index
    gradeAvgsAsJson = DataProcesserCoupling::getGradeAvgPerClassAsJSON
    #make this a method and return
    parsedGradeAvgs = JSON.parse(gradeAvgsAsJson)
    @avgGradePerClass = parsedGradeAvgs

    startedExcersisesAmount = DataProcesserCoupling::getAmountOfStartedExcersisesPerStudentAsJSON
    parsedExcersiseAmounts = JSON.parse(startedExcersisesAmount)
    @excersisesAmounts = parsedExcersiseAmounts


    gradesAndExcerisesClustersJSON = '{
  "0": "499",
  "5": "548",
  "25": "599",
  "37": "635",
  "50": "508",
  "75": "472",
  "100": "248"
}'
    #DataProcesserCoupling::getKMeansAsJSON
    gradesAndExcerisesClustersParsed = JSON.parse(gradesAndExcerisesClustersJSON)
    @gradesAndExcerisesClusters = gradesAndExcerisesClustersParsed

  end
end
