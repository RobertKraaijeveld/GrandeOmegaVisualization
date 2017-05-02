require_dependency 'dataprocesserglue'
require 'rubygems'
require 'json'

class HomeController < ApplicationController
  include DataProcesserCoupling
  helper_method :kmeans
  helper_method :successrate  

  def index
    gradeAvgsAsJson = DataProcesserCoupling::getDataProcesser.getGradeAvgPerClass
    #make this a method and return
    parsedGradeAvgs = JSON.parse(gradeAvgsAsJson)
    @avgGradePerClass = parsedGradeAvgs

    startedExcersisesAmount = DataProcesserCoupling::getDataProcesser.getAmountOfStartedExcersisesPerStudent
    parsedExcersiseAmounts = JSON.parse(startedExcersisesAmount)
    @excersisesAmounts = parsedExcersiseAmounts
  end

  def kmeans
    upperPercentageOfGradesToBeSelected = params.fetch(:percentage).to_i
    gradesAndExcerisesClustersJSON = DataProcesserCoupling::getDataProcesser.getKMeans(upperPercentageOfGradesToBeSelected) 

    gradesAndExcerisesClustersParsed = JSON.parse(gradesAndExcerisesClustersJSON)
    render json: gradesAndExcerisesClustersParsed
  end 

  def successrate
    #extract!
    upperPercentageOfGradesToBeSelected = params.fetch(:percentage).to_i
    studentsGradesAndSuccesRateJSON = DataProcesserCoupling::getDataProcesser.getSuccesRate(upperPercentageOfGradesToBeSelected)
    
    studentsGradesAndSuccesRateParsed = JSON.parse(studentsGradesAndSuccesRateJSON)
    render json: studentsGradesAndSuccesRateParsed
  end
end
